/**************************** sys_time.c ********************************/
/* Copyright 2003/12/28 Aeolus Development				*/
/* All rights reserved.							*/
/*									*/
/* Redistribution and use in source and binary forms, with or without	*/
/* modification, are permitted provided that the following conditions	*/
/* are met:								*/
/* 1. Redistributions of source code must retain the above copyright	*/
/*   notice, this list of conditions and the following disclaimer.	*/
/* 2. Redistributions in binary form must reproduce the above copyright	*/
/*   notice, this list of conditions and the following disclaimer in the*/
/*   documentation and/or other materials provided with the 		*/
/*   distribution.							*/
/* 3. The name of the Aeolus Development or its contributors may not be	*/
/* used to endorse or promote products derived from this software 	*/
/* without specific prior written permission.				*/
/*									*/
/* THIS SOFTWARE IS PROVIDED BY THE AEOULUS DEVELOPMENT "AS IS" AND ANY	*/
/* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE	*/
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR	*/
/* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AEOLUS DEVELOPMENT BE	*/
/* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR	*/
/* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF	*/
/* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 	*/
/* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,*/
/* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE */
/* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 	*/
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.			*/
/*									*/
/*  Time routines.  Polled implementations of routines to provide timed	*/
/* waits and elapsed time. 						*/
/*  Could be refined yet.						*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 sys_time.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 sys_time.c 17-Jan-2004,16:07:18,`RADSETT' Move max_microsecond storage and
*        init out of timing function
*        into initialization.
*   3 sys_time.c 29-Jan-2004,10:29:08,`RADSETT' Update StartClock minimum count
*        measurement to use a better comparison
*        Add section to StartClock to measure the overhead of a WaitUs call.
*        Change WaitUs to avoid attempting measurements with counts below
*        min_count.
*        Change WaitUs to reduce the requested wait time to compensate for call
*        overhead.
*        Change UsToCounts to replace /CLOCK_SPEED operation with shift and add
*        operations so that it takes constant time.  Add ifdef so it can be
*        switched
*        back to divide if CLOCK_SPEED is changed.
*   4 sys_time.c 14-Jun-2004,15:14:42,`RADSETT' Remove use of struct _reent
*        argument so this
*        can be decoupled from the newlib support.
*   TLIB revision history ends.
*/
#include <stdio.h>
#include <limits.h>
#include "lpc210x.h"
#include "lpc_sys.h"
#include "errno_lpc.h"

	/**** Local Macros ****/

	/*  If TIME_TO_COUNTS_USE_DIV is defined then the routines will	*/
	/* use a normal divide operation.  Otherwise the division will	*/
	/* be replaced with shift and add operations.  This is 		*/
	/* marginally faster.  More important it does the operation in	*/
	/* the same time no matter what the value used.  		*/
	/*  Note: If CLOCK_SPEED is changed then either			*/
	/* TIME_TO_COUNTS_USE_DIV must be defined OR UsToCounts and	*/
	/* divx must be changed to reflect the new value.		*/
/* #define TIME_TO_COUNTS_USE_DIV */
#define CLOCK_SPEED	(10000000)	/*  Nominal speed for internal 	*/
					/* clock in Hz.			*/

	/*  Used for converting between counts and uS for time function.*/
	/* An unsigned long long so overflows are not possible during 	*/
	/* the conversion.  Note relationship to CLOCK_SPEED.		*/
#define COUNTS_PER_US	(10uLL)

#define COUNTER_RESET	((unsigned char)2)    /* Hold counter in reset.	*/
#define COUNTER_ENABLE	((unsigned char)1)    /* Let counter run.	*/	

	/*  Used to control and detect matches.  Use R3 since it has no	*/
	/* matching external output on T0.				*/
#define MATCH_T0_R3_NOTHING	(~0xE00u)/*  Do nothing when match 	*/
 					/* register 3 matches T0.	*/
#define EXTERN_MATCH_T0_R3	(~0xC08u)/*  Set External output low (if */
					/* connected), do nothing on 	*/
					/* match.			*/
#define EXTERN_MATCH_T0_R3_SET	(0x800)	/*  Set external output high	*/
					/* (if connected) on match.	*/

	/*** Local Variables ***/

	/*  Scales between desired nominal clock frequency and what we	*/
	/* were actually able to achieve.  This ends up being the 	*/
	/* actual clock rate in Hz.					*/
static unsigned long timing_scale_factor;	

	/*  Provides high order timing bits so internal timer can 	*/
	/* record a running time  longer than ~.12 Hrs.  In fact this	*/
	/* allows a representation of > 50,000 years clearly (I hope)	*/
	/* unreasonably long.						*/
static unsigned long high_counts = 0uL;

	/*  Maximum number of microseconds that a single timing 	*/
	/* operation can handle.  Larger values have to be split into 	*/
	/* multiple operations.						*/
static unsigned int max_microsec = 0;

	/*  Minimum number of counts a core timing operation takes.	*/
static unsigned int min_count = 0;

	/*  Minimum number of counts a WaitUs call takes.		*/
static unsigned int count_overhead = 0;

	/**** Internal Prototypes ****/

/********************* counts_to_us *************************************/
/*  counts_to_us -- converts from internal units in counts to uS.  	*/
static unsigned int counts_to_us( unsigned int counts);

/********************* divx *********************************************/
/*  Divide by 78125.  Constant time operation.				*/
static unsigned long long divx(unsigned long long x);

/********************* accumulate_time **********************************/
/*  accumulate_time -- Check for rollover of HW timer and increments	*/
/* the high order count if it is detected.  Must be called at at least	*/
/* twice the rollover frequency of the HW timer to ensure proper 	*/
/* detection of rollover.  That works out to about once every 3.5 	*/
/* minutes at nominal clock rate.					*/
static void accumulate_time( void);

/********************* get_full_counts **********************************/
/*  get_full_counts -- Concatenate the stored high counts and the HW	*/
/* timer low counts to get a large number containing the full count.	*/
static unsigned long long get_full_counts( void);

	/**** Implementation ****/

/********************* StartClock ***************************************/
/*  StartClock -- Starts up the clock used for internal timing.  	*/
/* Attempts to match the desired clock speed (CLOCK_SPEED) and 		*/
/* initializes timing_scale_factor to a compensating scale.  Returns 0	*/
/* if successful, otherwise error code will be retained in errno.	*/
/*  Note:  Should be called only after all clocks have been set up.	*/
/* Otherwise time scale will not be correct.				*/
/*  Returns 0 if successful.						*/
int StartClock( void)
{
 unsigned long rate, divider;
 unsigned long lclmax_microsec = 0uL;
 unsigned int start_count, end_count;
 unsigned int test_us;

 rate = VPBRate();		/*  Get the clock rate of the		*/
				/* peripheral bus.			*/
 if( rate == 0) {		/*  Internal error, complain and exit.	*/
    return ELPC_INTERNAL;
    }

	/* Set clock to reset.  This will keep it at zero.		*/
 T0TCR = COUNTER_RESET;

	/*  Calculate a divider that will cause the internal clock to	*/
	/* approximate the target clock speed.  This should give a 	*/
	/* clock counting rate between the desired rate and twice the	*/
	/* desired rate.						*/
 divider = rate/ CLOCK_SPEED;
 if( divider == 0) {	/*  Sanity check on the divider.  If we have a	*/
    divider = 1uL;	/* pclk slower than our desired rate we won't	*/
    }			/* be able to reach the desired rate so just	*/
			/* run as fast as we can.			*/
 T0PR = divider - 1;	/*  Set prescaler to give us our target rate.	*/

	/*  Since the actual clock rate we have and our target rate may	*/
	/* not be the same, calculate a corrective factor for routines	*/
	/* that make use of it.  This value will be divided by 		*/
	/* CLOCK_SPEED when dealing with time and so should produce a	*/
	/* result precise to 1 part in CLOCK_SPEED.			*/
 timing_scale_factor = rate / divider;

	/* Set to do nothing on match, and set output to 0.		*/
 T0MCR = (unsigned short)(T0MCR & MATCH_T0_R3_NOTHING); 
 T0EMR = (unsigned short)(T0EMR & EXTERN_MATCH_T0_R3);

 T0TCR = COUNTER_ENABLE;	/* Enable clocks.			*/

 if( max_microsec == 0) {		/*  Find saturation point.	*/
    lclmax_microsec = counts_to_us( UINT_MAX);
    max_microsec = lclmax_microsec;
    }

	/*  Find the minimum number of counts it takes to set up and 	*/
	/* detect a time period.  This simulates the normal timer 	*/
	/* operation.  The purpose is to avoid any possible missed time	*/
	/* periods by shortcutting the time wait process if the time	*/
	/* would have passed by the time we had performed the operation	*/
	/* anyway.  							*/
	/*  We deliberately set out to slightly overestimate the time 	*/
	/* involved to avoid the danger of underestimating.  This 	*/
	/* overestimation occurs in three ways:				*/
	/*   1 - Operations that would normally occur after a match 	*/
	/*       would be detected are included in the count.		*/
	/*   2 - One count is added so that any fractional counts will 	*/
	/*       be compensated for.					*/
	/*   3 - The operations that do the counting are invariably	*/
	/*       included.						*/
	/*  Note that we are at the tender mercies of the compiler not 	*/
	/* to generate significantly different code for this and the 	*/
	/* code used for actual timing.					*/
 start_count = T0TC;		/*  Starting time.			*/

	/*  This main loop is copied from WaitUs and modified in a 	*/
	/* fashion to try and preserve all the operations in a fashion 	*/
	/* that the compiler will not optimize this passage differently	*/
	/* than the 'real' version.  Comments from the original version	*/
	/* are kept with notes added to indicate what operations are	*/
	/* performed.							*/

	/*  First perform a comparison of a local variable against	*/
	/* global variable. Note:  this comparison assumes that 	*/
	/* high_counts is still zero.  If StartClock is run more than	*/
	/* once between resets this code may fail to operate correctly.	*/
 if( lclmax_microsec > high_counts) { 

	/*  Add a local variable to current time counter and store in 	*/
	/* match register.						*/
    T0MR3 = T0TC + start_count;	/*  Set match to current time + delay	*/
				/* time.				*/

	/*  Exact match for 'real' operation.				*/
		/*  Set operation on match to set to 1.			*/
    T0EMR =  (unsigned short)(T0EMR | EXTERN_MATCH_T0_R3_SET);

	/*  Compare external register to local.  This does not match 	*/
	/* the original operation which masks the external register and	*/
	/* compares it to zero.  The hope is that the operations will 	*/
	/* be close enough together that any differences (particularly	*/
	/* short counts) will be masked by the over-estimation 		*/
	/* precautions.  Since the difference between the two is only 	*/
	/* an instruction or two this appears to be a reasonable 	*/
	/* assumption.							*/
    while( T0TC  == start_count) {	/*  Wait for match to set output*/
         }				/* to 1.			*/

	/*  Exact match for 'real' operation.				*/
		/*  Set external match 0 output to 0 and operation on	*/
		/* match to do nothing.					*/
		/*  Set external match 0 output to 0 and operation on 	*/
		/* match to do nothing so we don't trigger next wait	*/
		/* early by accident.					*/
    T0EMR = (unsigned short)(T0EMR & EXTERN_MATCH_T0_R3);
    }
	/*  End of the copied main loop.			 	*/

 end_count = T0TC;	/*  Ending time.				*/

	/*  Calculate the counts needed to perform a timing operation.	*/
	/* Add a count for safety.					*/
 min_count = (end_count - start_count) + 1;

	/*  Now find overhead for a wait call.  This will be used to	*/
	/* compensate for that overhead to increase the accuracy at 	*/
	/* short waits.  We do this by waiting for a short period and	*/
	/* measuring how long it actually takes.			*/

	/*  First figure out how long to wait.  Want as short as 	*/
	/* possible so we wait for the minimum useable period which 	*/
	/* will be the minimum detectable plus 1 microsecond		*/
 test_us = counts_to_us(min_count) + 1u;

 start_count = T0TC;		/*  Starting time.			*/
 WaitUs( test_us);
 end_count = T0TC;		/*  Ending time.			*/

	/*  Total time spent.  Add 1/2 of a microsecond for rounding.	*/
 count_overhead = (end_count - start_count) - UsToCounts( test_us);

 return 0;
}

/********************* counts_to_us *************************************/
/*  counts_to_us -- converts from internal units in counts to uS.  	*/
/*  unsigned int counts	-- # of counts to be converted. 		*/
/*  Returns number of microseconds corresponding to counts.  Truncates	*/
/* result to microsecond resolution.					*/
static unsigned int counts_to_us( unsigned int counts)
{
 unsigned int us;

	/*  Convert the counts to microseconds taking the actual clock 	*/
	/* rate into account.  Note that as long as 			*/
	/* CLOCK_SPEED/(timing_scale_factor * COUNTS_PER_US) is less	*/
	/* than 1 this cannot overflow a long.  With nominal CLOCK_SPEED*/
	/* at 10000000 this will be true as long as the actual clock	*/
	/* speed is > 1000000.						*/
 us = (unsigned int)((counts * (unsigned long long)CLOCK_SPEED)/
    (timing_scale_factor * COUNTS_PER_US));

 return us;
}

#ifdef TIME_TO_COUNTS_USE_DIV

/********************* UsToCounts ***************************************/
/*  UsToCounts -- converts to internal units in counts from uS.  Other 	*/
/* Modules use this counter for a timebase so this needs to be 		*/
/* available to them.							*/
/*  unsigned int us	-- microseconds to convert to counts.	 	*/
/*  Returns number of counts corresponding to us.  Saturates on		*/
/* overflow so for large time periods it is possible to get a result	*/
/* lower than requested.						*/
unsigned int UsToCounts( unsigned int us)
{
 unsigned long long counts;

	/*  Convert the nanoseconds to counts taking the actual clock 	*/
	/* rate into account.  Note: NS_PER_COUNT * CLOCK_SPEED should 	*/
	/* always be 1,000,000,000.  The expanded form just shows the 	*/
	/* intent more clearly.  					*/
 counts = (COUNTS_PER_US * us * timing_scale_factor)/(CLOCK_SPEED);

 if( counts > ULONG_MAX) {
    return ULONG_MAX;
    }
 return (unsigned int)counts;
}

#else

/********************* divx *********************************************/
/*  Divide by 78125.  Constant time operation.				*/
static unsigned long long divx(unsigned long long x)
{
 unsigned long long res;

	/*  None of these operations overflow only since before this	*/
	/* routine is called the calling routine uses a shift operation	*/
	/* to perform a divide by 128 to provide enough headroom to	*/
	/* allow this to succeed w/o overflow.				*/
 res = (84*x)>> 7;
 res = (73*x - res)>> 7;
 res = (94*x + res) >> 8;
 res = (121*x + res) >> 7;
 res = (106*x + res) >> 9;
 res = (101*x + res) >> 7;
 res = (95*x + res) >> 8;
 res = (107*x + res) >> 23;
 return res;
}

/********************* UsToCounts ***************************************/
/*  UsToCounts -- converts to internal units in counts from uS.  Other 	*/
/* Modules use this counter for a timebase so this needs to be 		*/
/* available to them.  Replaces division operation with an equivalent	*/
/* series of multiplies, shifts and adds.				*/
/*  unsigned int us	-- microseconds to convert to counts.	 	*/
/*  Returns number of counts corresponding to us.  Saturates on		*/
/* overflow so for large time periods it is possible to get a result	*/
/* lower than requested.						*/
unsigned int UsToCounts( unsigned int us)
{
 unsigned long long tmp;

 tmp = COUNTS_PER_US * us * timing_scale_factor;
 tmp >>= 7;		/*  First divide by 128.  Doing this first	*/
 			/* ensures 7 bits of overhead for later divide	*/
 			/* by 5 operations.				*/

 tmp = divx(tmp);

 if( tmp > ULONG_MAX) {
    return ULONG_MAX;
    }
 return (unsigned int)tmp;
}

#endif	/*  TIME_TO_COUNTS_USE_DIV */

/********************* accumulate_time **********************************/
/*  accumulate_time -- Check for rollover of HW timer and increments	*/
/* the high order count if it is detected.  Must be called at at least	*/
/* twice the rollover frequency of the HW timer to ensure proper 	*/
/* detection of rollover.  That works out to about once every 3.5 	*/
/* minutes at nominal clock rate.					*/
static void accumulate_time( void)
{
 static unsigned int last_time = 0;
 unsigned long current_time;

	/*  A simple polled rollover check.  This should work as long as*/
	/* this routine is called at least once every time the counter	*/
	/* has incremented 1/2 of its maximum count. 			*/
 current_time = T0TC;			/*  Get the current count	*/
 if(current_time < last_time) {		/*  If we have rolled over, 	*/
    high_counts++;			/* increment the high order	*/
    }					/* accumulator.			*/
 last_time = current_time;		/*  Record last read value for	*/
					/* the next check.		*/
}

/********************* get_full_counts **********************************/
/*  get_full_counts -- Concatenate the stored high counts and the HW	*/
/* timer low counts to get a large number containing the full count.	*/
/*  Returns number of counts since start.				*/
static unsigned long long get_full_counts( void)
{
 unsigned long low;
 unsigned long long full_count;


	/*  Read in the clock counts.  It is necessary to perform a 	*/
	/* rollover check so we don't end up with the low order value 	*/
	/* that of after the rollover and the high order count from 	*/
	/* before the rollover.  If that were to happen our time would	*/
	/* not increase in a monotonic fashion. 			*/
 do {
    accumulate_time();		/*  Check for rollover of HW clock.	*/
    low = T0TC;		/*  Read current count.				*/

	/*  Form low order and high order counts into a single value.	*/
    full_count = ((unsigned long long)high_counts << 32) | low;

    } while( low > T0TC);	/*  Check for rollover and redo if one	*/
				/* has happened.			*/
 return full_count;		/*  All done.				*/
}

/********************* WaitUs *******************************************/
/*  WaitUs -- Wait for 'wait_time' us					*/
/*  unsigned long wait_time	-- microseconds to convert to counts. 	*/
/*  Will break wait into multiple waits if needed to avoid saturation.	*/
void WaitUs( unsigned int wait_time)
{
 unsigned int counts;

 accumulate_time();		/*  Check for rollover of HW clock.	*/

 	/*  Convert from requested wait in uS to counts used by the HW	*/
 	/* counter.  If necessary break into multiple waits.		*/
 while( wait_time > 0) {
    if( wait_time > max_microsec) {
         counts = UsToCounts( max_microsec);
	 wait_time -= max_microsec;
	 }

	/*  Note that we always execute the else portion of this test	*/
	/* once for every call to WaitUs.				*/
    else {
         counts = UsToCounts( wait_time);

		/*  Compensate for overhead.  Subtract out overhead for	*/
		/* waits that are long enough.  For shorter periods	*/
		/* simply minimize the wait.				*/
	 if( counts > count_overhead) {
	      counts -= count_overhead;
	      }
	 else {
	      counts = 0;
	      }
	 wait_time = 0;
	 }

	/*  If counts is lower than min_count zero then the wait	*/
	/* interval is too small and we should just return.  The 	*/
	/* smallest count we can deal with is the count that occurs 	*/
	/* between reading the HW clock	and setting the match register.	*/
	/* The value of this minimum will depend on the timer rate and	*/
	/* the CPU execution speed.					*/
    if( counts > min_count) { 

         T0MR3 = T0TC + counts;	/*  Set match to current time + delay	*/
				/* time.				*/

		/*  Set operation on match to set to 1.			*/
         T0EMR =  (unsigned short)(T0EMR | EXTERN_MATCH_T0_R3_SET);

         while( (T0EMR & 0x8) == 0) {	/*  Wait for match to set output*/
              }				/* to 1.			*/

		/*  Set external match 0 output to 0 and operation on	*/
		/* match to do nothing.					*/
		/*  Set external match 0 output to 0 and operation on 	*/
		/* match to do nothing so we don't trigger next wait	*/
		/* early by accident.					*/
         T0EMR = (unsigned short)(T0EMR & EXTERN_MATCH_T0_R3);
         }

    accumulate_time();		/*  Check for rollover of HW clock.	*/
    }
}					      

/********************* GetUs ********************************************/
/*  GetUs -- Get the current time in uS.				*/
unsigned long long GetUs( void)
{
 unsigned long long counts, us;

 counts = get_full_counts();	/*  Get the total passed time sice 	*/
 				/* counter start.			*/
	/*  Convert low order of timer to nS.				*/
 us = counts_to_us( (unsigned int)(counts & UINT_MAX));
						
 counts >>= 32;			/*  Now get the high order count.	*/

	/*  Given a 64 bit number uS resolution provides a range 	*/
	/* exceeding 500,000 years.  I think we'll skip overflow	*/
	/* detection. 							*/
 us += counts_to_us(UINT_MAX) * counts;
 return us;
}

/********************* MinimumAchievableWait ****************************/
/*  MinimumAchievableWait -- Get the shortest wait we can do.		*/
unsigned int MinimumAchievableWait(void)
{
 return counts_to_us( count_overhead + min_count + (UsToCounts( 1u)/2));
}

/* set up a timer interrupt to make sure the stack didn't grow too much */
/* this is mainly for debugging, call memcheck_init() at startup, fill
   in values for MIN and MAX, and memory usage statistics will be
   send via printf */

#define MAGIC 0x55
#define MAX ((char*)0x4ff) /* set to bottom of stack */
#define MIN ((char*)0x260) /* set to end of bss */

#define SIMPLE_DETECTION /* just check bottom of stack */

/* calculate free space */
ISR(TIMER0_OVF_vect) {
#ifdef SIMPLE_DETECTION
   if(*MIN != MAGIC) {
      DEBUG("STACK OVERFLOW DETECTED\n");
      timer_stop();
      sei();
      for(;;);
   }
#else
   static int r, maxstack;
   if(++r < 1000)
      return;
   r = 0;

   char *p;
   for(p = MIN; p <= MAX; p++)
      if(*p != MAGIC)
         break;

   int usage;
   usage = MAX - p;
   if(usage > maxstack) {
      DEBUG("Max Stack Usage: %d\n", usage);
      maxstack = usage;
   }
   
   if(usage == MAX - MIN) {
      DEBUG("STACK OVERFLOW DETECTED\n");
      timer_stop();
      sei();
      for(;;);
   }

   //   for(p = MIN; p < (char*)&usage; p++)
   // *p = MAGIC;
#endif
}

/* if addr is written different from 0, we are out of memory */
void memcheck_init()
{
#ifdef SIMPLE_DETECTION
   *MIN = MAGIC;
#else
   char *p;
   for(p = MIN; p < (char*)&p; p++)
      *p = MAGIC;
#endif

    timer_init();
}

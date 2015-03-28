/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* Crystal frequence,10MHz~25MHz should be the same as actual status. */
#define Fosc	12000000UL

/* System frequence,should be (1~32)multiples of Fosc,and should be equal or 
less than 60MHz. */
#define Fcclk	(Fosc * 5)

/* CCO frequence,should be 2/4/8/16 multiples of Fcclk, ranged from 156MHz to 
320MHz. */
#define Fcco	(Fcclk * 4)

/* VPB clock frequence , must be 1/2/4 multiples of (Fcclk / 4). */
//#define Fpclk	(Fcclk / 4) * 1


#define Fpclk	(Fcclk) * 1

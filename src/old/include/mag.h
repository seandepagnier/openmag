static inline void mag_reset(void)
{
   /* toggle reset io */
   MAGRESETPORT |= MAGRESET;
   MAGRESETPORT &= ~MAGRESET;
}

#define PS0 16
#define PS1 32
#define PS2 64

static inline void mag_command(int axis)
{
   unsigned char cmd = 0;

   switch(axis) {
      case 0: cmd |= 2; break;
      case 1: cmd |= 1; break;
      case 2: cmd |= 3; break;
   }

   cmd |= PS0 | PS1 | PS2;

   spi_transfer(cmd);
}

static inline int mag_read(void)
{
   uint8_t a = spi_transfer(0);
   uint8_t b = spi_transfer(0);
   return ((uint16_t)a << 8) | b;
}

static volatile int32_t magdata[3];
static volatile uint8_t magaxis;

static inline void mag_getnextaxis()
{
   mag_reset();
   //   spi_setss();
   //   _delay_ms(1);
   mag_command(magaxis);
}

/* in AUTO_UPDATE mode, magdata gets updated all the time
   don't care if it's being used faster or slower than that rate */

/* mag data ready interrupt */
ISR(PCINT0_vect)
{
   /* don't care about falling edge interrupt */
   if(!(MAGDRDYPORT & MAGDRDY))
      return;

   static int32_t magcur[3];
   static int8_t curcount;

   magcur[magaxis] += mag_read();

   magaxis++;

   if(magaxis == 3) {
      magaxis = 0;

      if(++curcount == 1) {
         magdata[0] = magcur[0];
         magdata[1] = magcur[1];
         magdata[2] = magcur[2];
         magcur[0] = magcur[1] = magcur[2] = 0;
         curcount = 0;
      }
   }

   mag_getnextaxis();
}

/* push out last 3 values, and start reading again */
#ifndef MAG_AUTOUPDATE
static void mag_readdata(int val[3])
{
   while(magaxis < 3); /* wait until complete */

   val[0] = magdata[0];
   val[1] = magdata[1];
   val[2] = magdata[2];

   magaxis = 0;
   mag_getnextaxis();
}
#endif

static void mag_init(void)
{
   /* set data ready as input */
   MAGDRDYDDR &= ~MAGDRDY;

   /* set up data ready interrupt */
   PCMSK0 = MAGDRDYINT;
   PCICR |= _BV(PCIE0);

   /* set reset as output */
   MAGRESETDDR |= MAGRESET;

   /* start chain reaction */
   mag_getnextaxis();
}

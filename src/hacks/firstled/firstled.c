#include <avr/interrupt.h>
#define F_CPU 1000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

static long x;

void mydelay(int ms)
{
   _delay_ms(ms);
}

int main(void){

   DDRB = ~0; //_BV(PB0) | _BV(PB1);

   for(;;) {
     PORTB ^= _BV(PB0);
     //     if(PIND & _BV(PD0))
        PORTB ^= _BV(PB1);

        mydelay(100);
  }

  return(0);
}

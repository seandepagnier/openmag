#ifdef BOOTLOADER_OFFSET

#if   USB_INT_NUM == 0
#define USB_INT_vect INT0_vect
#define USB_INT INT0
#define USB_ISC0 ISC00
#define USB_ISC1 ISC01
#elif USB_INT_NUM == 1
#define USB_INT_vect INT1_vect
#define USB_INT INT1
#define USB_ISC0 ISC10
#define USB_ISC1 ISC11
#elif USB_INT_NUM == 2
#define USB_INT_vect INT2_vect
#define USB_INT INT2
#define USB_ISC0 ISC20
#define USB_ISC1 ISC21
#else
#error "unknown interrupt"
#endif

ISR(USB_INT_vect)
{
   /* disable any and all interrupts we set in the app */
   TIMSK0 &= ~_BV(TOIE0);

   ((void (*)(void))BOOTLOADER_OFFSET)();
}

void usbloadable_init(void)
{
   EIMSK &= ~_BV(USB_INT); /* disable interrupt */

   DDRD &= ~_BV(PD2); /* set D+ to input */
   PORTD |= _BV(PD2); /* set weak pullup */

   EIMSK = _BV(USB_INT); /* enable interrupt */
   EICRA |= _BV(USB_ISC0) | _BV(USB_ISC1);
}
#endif

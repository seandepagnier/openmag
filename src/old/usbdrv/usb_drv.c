// AT90USB/usb_drv.c
// Basic functions for Atmels AT90USB microcontrollers
// Based on Atmels AT90USB datasheet 7593D-AVR-07/06
// S. Salewski 21-MAR-2007

#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h> // sei()
#include <usb_drv.h>

#include <usb_api.h> // EP0_FIFO_Size 

#include <usb_isr.h>

#include <debug.h>

uint8_t UsbAllocatedEPs = 0;
volatile uint8_t UsbStartupFinished = 0;

// Set some registers to their initial (reset) value.
// Reason: Atmels bootloader activates some interrupts.
// This function deactivates it, so we have clean start
// conditions if our program is started from bootloader.
void
UsbInitialReset(void)
{
  USBCON	= (1<<FRZCLK);
  OTGIEN	= 0;
  UDIEN		= 0;
  UHIEN		= 0;
  UEIENX	= 0;
  UPIENX	= 0;
  UHWCON	= (1<<UIMOD);
}

// Start PLL and enable clock
void
UsbStartPLL(void)
{
  UsbSetPLL_XTAL_Frequency();
  UsbEnablePLL();
  UsbWaitPLL_Locked();
  UsbEnableClock();
}

// Basic USB activation necessary to trigger a wakeup interrupt
// Wakeup ISR will start PLL clock, then USB-END-OF-RESET interrupts are recognized
void
UsbDevLaunchDevice(bool lowspeed)
{
  UsbInitialReset();
  if (0) // set it to (1) if you need very small code size (i.e. bootloader, saves 78 bytes)
  {
    UHWCON = ((1<<UIMOD) | (1<<UVREGE));
    USBCON = ((1<<USBE) | (1<<OTGPADE) | (1<<FRZCLK));
    asm volatile ("nop"); // nop may be are necessary if mpu is clocked with 16 MHz (deactivated prescaler)
    // asm volatile ("nop");
    if (lowspeed) UDCON = (1<<LSM);
    USBCON = ((1<<USBE) | (1<<OTGPADE));
    USBCON = ((1<<USBE) | (1<<OTGPADE) | (1<<FRZCLK)); // setting FRZCLK again in not necessary (see below)
    UDIEN = ((1<<WAKEUPE) | (1<<EORSTE));
  }
  else // do the same thing, bit for bit, with named macros
  {
#ifdef USE_PADS_REGULATOR  //pads regulator should be used if the supply voltage is above 3.6
    UsbEnablePadsRegulator();
#endif
    UsbEnableController();
    UsbEnableOTG_Pad(); // necessary to power on the device (undocumented feature)
    UsbDisableUID_ModeSelection();
    UsbSetDeviceMode();
    UsbSetDeviceModeReg();
    if (lowspeed) UsbDevSelectLowSpeed(); // fullspeed is default
    UsbEnableClock(); // without this no (wakeup) interrupt is triggered!!!
    UsbFreezeClock(); // FreezeClock() is not really necessary, but we may feel better because PLL in not running yet
    cli();
    UsbDevEnableWakeupCPU_Int();
    UsbDevEnableEndOfResetInt(); // call this AFTER (UsbEnableController(); UsbEnableOTG_Pad();)
    UsbEnableVBUS_TransitionInt();
  }
  UsbDevAttach();
}

// Called by USB-END-OF-RESET interrupt
// USB reset resets EP0! (see section 22.5)
void
UsbDevStartDeviceEP0(void)
{
   USB_DEBUG("UsbDevStartDeviceEP0\n");
  UsbDevSelectEndpoint(0);
  if UsbDevIsEndpointEnabled() // can this be true?
   USB_DEBUG("~~~ EP0 already enabled!\n");
  else
  {
   USB_DEBUG("~~~ Setting up EP0...\n");
    UsbAllocatedEPs = 0;
    if (UsbDevEP_Setup(0, UsbEP_TypeControl, EP0_FIFO_Size, 1, UsbEP_DirControl)) 
     USB_DEBUG("~~~ Successful set up EP0!\n");
    else
     USB_DEBUG("~~~  Setup of EP0 failed!\n");
  }
  UsbDevEnableReceivedSETUP_Int();
}

// To reduce codesize, you may comment this function out and allocate your ep with low level macros
// num: 0...6
// type: UsbEP_TypeControl, UsbEP_TypeIso, UsbEP_TypeBulk, UsbEP_TypeInterrupt
// size: 8, 16, 32, 64. Only for isochronous ep 128, 256 or 512.
// banks: 1 or 2
// dir: UsbEP_DirOut, UsbEP_DirControl, UsbEP_DirIn
// more than one control ep or more than one bank for control ep0 may work, but is not recommended
bool
UsbDevEP_Setup(uint8_t num, uint8_t type, uint16_t size, uint8_t banks, uint8_t dir)
{ 
  uint8_t i, j;
  banks--;
  if ((num > 6) || (dir > 1) || (banks > 1) || (type > 3) || (size & 0xFC07)) return false;
  i = (uint8_t) (size / 8);
  if ((UsbDevIsLowSpeedSelected()) && ((type == UsbEP_TypeBulk) || (type == UsbEP_TypeIso) || (i > 1))) return false;
  if (!((i == 1) || (i == 2) || (i == 4) || (i == 8)))
    if (!(((i == 16) || (i == 32) || (i == 64)) && (type == UsbEP_TypeIso))) return false;
  if ((type == UsbEP_TypeControl) && ((dir != UsbEP_DirControl) || (banks > 0))) return false;
  if (num == 0)
  {
    if ((dir != UsbEP_DirControl) || (type != UsbEP_TypeControl)) return false; // EP0 is always control ep
  }
  else
  {
    if ((num != UsbAllocatedEPs) || // allocate eps in growing order, see section 21.7
        (type == UsbEP_TypeControl)) return false; // more than one control ep may be ok?
  }
  UsbDevSelectEndpoint(num);
  UsbDevEnableEndpoint(); // enable ep before memory is allocated? Yes, see figure 22-2
  j = 0;
  while ((i = (i >> 1))) j++;
  UECFG0X = ((type << 6) | (dir));
  UECFG1X = ((j << 4) | (banks << 2));
  UECFG1X |= (1 << ALLOC);
  if (UESTA0X & (1 << CFGOK))
  {
    UsbAllocatedEPs++;
    return true;
  } 
  else
  {
    UsbDevDisableEndpoint();
    return false;
  }
}

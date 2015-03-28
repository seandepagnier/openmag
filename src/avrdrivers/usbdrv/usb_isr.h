// AT90USB/usb_isr.c
// USB Interrupt Service Routines
// S. Salewski 22-MAR-2007

#include <avr/interrupt.h>
#include <stdint.h>
#include <usb_drv.h>

#include <debug.h>

#include <usb_api.h>
#include <usb_requests.h>

void usb_rxline_poll(void);
void usb_flush_rb(void);

// USB General Interrupt Handler (Figure 21.11)
// USB Registers: USBINT.0, USBINT.1, UDINT
ISR(USB_GEN_vect)
{
  USB_DEBUG("ISR(USB_GEN_vect)\n");
  if (UsbIsIDTI_FlagSet())
  {
      UsbClearIDTI_Flag();
      USB_DEBUG("=== IDTI_FlagSet\n");
  }

  if (UsbIsVBUSTI_FlagSet())
  {
      if(UsbIsVBUS_PinHigh()) {
          USB_DEBUG("=== Enable clock\n");
          UsbEnableClock();
          extern void usb_rxline_flush(void);
//          usb_rxline_flush();
      } else {
          USB_DEBUG("=== Disable clock\n");
          UsbFreezeClock();
      }

      UsbClearVBUSTI_Flag();
      USB_DEBUG("=== VBUSTI_FlagSet\n");
  }

  if (UsbDevIsEndOfResetFlagSet())
  {
      UsbDevClearEndOfResetFlag();
      UsbDevStartDeviceEP0();
      USB_DEBUG("=== EndOfResetFlagSet\n");
  }
  if (UsbDevIsWakeupCPU_FlagSet())
  {
      UsbDevDisableWakeupCPU_Int();
      UsbDevClearWakeupCPU_Flag();
      UsbStartPLL();
      USB_DEBUG("=== WakeupCPU_FlagSet\n");
  }
}

// USB Endpoint/Pipe Interrupt Handler (Figure 21.12)
// Endpoint Registers: UEINTX, UESTAX.6 and UESTAX.5
// Setup-Request may reset endpoints, so we process data endpoints first!
// User defined actions have to acknowledge the interrupt!
ISR(USB_COM_vect)
{
  uint8_t mask;
  uint8_t ep;

  mask = UsbDevGetEndpointIntBits();
  ep = UsbNumEndpointsAT90USB;
  while (ep-- > 0)
  {
    if (mask & (1<<ep))
    {
//      USB_DEBUG("ISR(USB_COM_vect) %d\n", ep);

      UsbDevSelectEndpoint(ep);
      switch (ep)
      {
        case 0:
          if (UsbDevHasReceivedSETUP())
            UsbProcessSetupRequest();
          break;
        case 1:
            UsbDevEP1IntAction();
          break;
        case 2:
            UsbDevEP2IntAction();
          break;
        case 3:
            UsbDevEP3IntAction();
          break;
        case 4:
            UsbDevEP4IntAction();
          break;
        case 5:
            UsbDevEP5IntAction();
          break;
        case 6:
            UsbDevEP6IntAction();
          break;
        default:
         USB_DEBUG("Error in ISR(USB_COM_vect)\n");
      }
    }
  }
}


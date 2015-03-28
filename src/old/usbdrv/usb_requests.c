// AT90USB/usb_requests.c
// Handling of USB (standard) requests
// S. Salewski 22-MAR-2007

#include <stdint.h>

#include <debug.h>

#include "macros.h"
#include <usb_drv.h>

#include <usb_spec.h>
#include <usb_api.h> // EP0_FIFO_Size and USB_NumInterfaces
#include <usb_requests.h>

uint8_t UsbDevConfValue = UsbUnconfiguredState;
uint8_t AltSettingOfInterface[1] = {UsbInterfaceUnconfigured};//, UsbInterfaceUnconfigured, UsbInterfaceUnconfigured, UsbInterfaceUnconfigured}; 
uint8_t RemoteWakeupActive = 0;

static void UsbDevWriteDescriptor(void *d, uint8_t *written, uint8_t requested);
static void UsbDevReadBytesN(void *c, uint8_t n);
static bool UsbSendDescriptors(uint8_t cdi, uint8_t *written, uint8_t requested);

// Send descriptor to host, but don't send more total bytes than requested.
// Stop if there is a ZLP (abort) from host or if all bytes are send, else
// fill FIFO and send FIFO if FIFO is full. 
static void
UsbDevWriteDescriptor(void *d, uint8_t *written, uint8_t requested)
{
  uint8_t l;
  l = requested - *written; // we may send l bytes more
  if (l > *(uint8_t*)d) l = *(uint8_t*)d; // clip to size of this descriptor
  while (l--)
  {
    if(UsbDevHasReceivedOUT_Data()) // ZLP from host -- abort
    {
      *written = requested; // prevent further writes
      return;
    }
    UsbDevWriteByte(*(uint8_t*)d++); // put byte to FIFO
    if (((*written+=1) % EP0_FIFO_Size) == 0) // if FIFO full
    {
      UsbDevSendControlIn(); // send FIFO
      //while (!(UsbDevHasReceivedOUT_Data() || UsbDevTransmitterReady()));
      while (!(UEINTX & ((1<<RXOUTI) | (1<<TXINI)))); // the same -- saves a few bytes
    }
  }
}

// Read n bytes from FIFO; FIFO should contain exactly n bytes
// Limited to n < 256
static void
UsbDevReadBytesN(void *c, uint8_t n)
{
  ASSERT(UsbDevGetByteCountLow() == n);
  while (n--) *(uint8_t*)c++ = UsbDevReadByte();
}

static void
WaitZLP_FromHost(void)
{
  while (!UsbDevHasReceivedOUT_Data());
  UsbDevClearHasReceivedOUT_Data();
}
 
void
UsbProcessSetupRequest(void)
{
  USB_DeviceRequest req;
  union // we can use the same piece of memory for these descriptors
  {
    USB_DeviceDescriptor devDes;
    USB_ConfigurationDescriptor confDes;
    char strDes[USB_MaxStringDescriptorLength];
  } u;
  uint8_t b;
  UsbDevSelectEndpoint(0);
  UsbDevReadBytesN(&req, 8);
  // Caution: We have to delay the AcknowledgeSETUP() if request is a 3 stage-transfer with out data
  // because host may send out data immediately after our acknowledge and may not see our stall request!
  if (!(req.bRequest == USB_StdDevReqSET_DESCRIPTOR)) // ! 3 stage-transfer with out data
    UsbDevAcknowledgeSETUP();
  UsbDumpSetupRequest(&req);
  if (UsbIsVendorRequest(req.bmRequestType))
  {
    USB_DEBUG("Received UsbVendorRequest\n");
    UsbDevProcessVendorRequest(&req);
    UsbDevSelectEndpoint(0);
    UsbDevSendControlIn(); // send ZLP
  }
  else if (UsbIsStandardRequest(req.bmRequestType))
  {
    USB_DEBUG("Received UsbStandardRequest\n");
    switch (req.bRequest)
    {
      case USB_StdDevReqGET_STATUS: // 3 stages with 2 byte IN-data -- not tested yet
        USB_DEBUG("USB_StdDevReqGET_STATUS\n");
        ASSERT(req.wValue == 0);
        ASSERT(req.wLength == 2);
        switch (req.bmRequestType)
        {
          case 128: // device: Self-Power-Bit, Remote-Wakeup-Bit set?
            ASSERT(req.wIndex == 0);
            if (UsbDevConfValue == UsbUnconfiguredState)
            {
              USB_DEBUG("Error: USB_StdDevReqGET_STATUS device in unconfigured state!\n");
              UsbDevRequestStallHandshake();
              return;
            }
            UsbGetConfigurationDescriptor(&u.confDes, UsbDevConfValue);
            if (u.confDes.bmAttributes & (1<<6)) b = 1; else b = 0; // self powered?
            if ((u.confDes.bmAttributes & (1<<5)) && (RemoteWakeupActive)) b |= (1<<1); 
            UsbDevWriteByte(b);
            break;
          case 129: // interface: Always return 0
            if (UsbDevConfValue == UsbUnconfiguredState)
            {
              USB_DEBUG("Error: USB_StdDevReqGET_STATUS interface in unconfigured state!\n");
              UsbDevRequestStallHandshake();
              return;
            }
            //ASSERT(req.wIndex < USB_Interfaces[UsbDevConfValue]);
            UsbDevWriteByte(0);
            break;
          case 130: // endpoint: is this endpoint stalled?
            ASSERT((MSB(req.wIndex) == 0));
            b = LSB(req.wIndex) & 127; // endpoint number
            if (b >= UsbNumEndpointsAT90USB)
            {
              USB_DEBUG("Error: USB_StdDevReqGET_STATUS endpoint does not exist!\n");
              UsbDevRequestStallHandshake();
              return;
            }
            if ((UsbDevConfValue == UsbUnconfiguredState) && (b > 0))
            {
              USB_DEBUG("Error: USB_StdDevReqGET_STATUS for ep > 0 in unconfigured state!\n");
              UsbDevRequestStallHandshake();
              return;
            }
            UsbDevSelectEndpoint(b);
            if (UsbDevIsEndpointStalled()) // stalled bit is marked write-only in datasheet -- do we need a separate state variable? 
              UsbDevWriteByte(1);
            else
              UsbDevWriteByte(0);
            UsbDevSelectEndpoint(0);
            break;
          default:
            USB_DEBUG("Error: USB_StdDevReqGET_STATUS!\n");
            UsbDevRequestStallHandshake();
            return;
        }
        UsbDevWriteByte(0);
        UsbDevSendControlIn();
        WaitZLP_FromHost();
        break;
      case USB_StdDevReqCLEAR_FEATURE: // 2 stages (no data-stage) -- not tested yet
      case USB_StdDevReqSET_FEATURE:
        USB_DEBUG("USB_StdDevReqCLEAR/SET_FEATURE\n");
        ASSERT(req.wLength == 0);
        switch  (req.bmRequestType)
        {
          case 0: // device
            ASSERT(req.wIndex == 0);
            if (req.wValue != 1) // Feature selector != DEVICE_REMOTE_WAKEUP 
            {
              USB_DEBUG("Error: USB_StdDevReqSET/CLEAR_FEATURE wrong feature selector for device!\n");
              UsbDevRequestStallHandshake();
              return;
            }
            if (req.bRequest == USB_StdDevReqCLEAR_FEATURE)
            {
              RemoteWakeupActive = 0;
            }
            else
            {
              UsbGetConfigurationDescriptor(&u.confDes, UsbDevConfValue);
              if (!(u.confDes.bmAttributes & (1<<5))) // Remote-Wakeup support set in configuration descriptor?
              {
                USB_DEBUG("Error: USB_StdDevReqSET_FEATURE, Remote-Wakeup not supported!\n");
                UsbDevRequestStallHandshake();
                return;
              }
              RemoteWakeupActive = 1; // here we only set a flag -- of course this is not enough to reactivate the sleeping bus
            } 
            break;
          case 1: // interface -- nothing to do. Should we request a stall handshake?
            USB_DEBUG("Error: USB_StdDevReqSET/CLEAR_FEATURE with receiver == interface!\n");
            UsbDevRequestStallHandshake();
            return;
            break;
          case 2: // endpoint
            ASSERT(MSB(req.wIndex) == 0);
            if (req.wValue != 0) // Feature selector != ENDPOINT_STALL
            {
              USB_DEBUG("Error: USB_StdDevReqSET/CLEAR_FEATURE wrong feature selector for endpoint!\n");
              UsbDevRequestStallHandshake();
              return;
            }
            b = LSB(req.wIndex) & 127; // endpoint address
            if (b >= UsbNumEndpointsAT90USB)
            {
              USB_DEBUG("Error: USB_StdDevReqSET/CLEAR_FEATURE endpoint does not exist!\n");
              UsbDevRequestStallHandshake();
              return;
            }
            if ((UsbDevConfValue == UsbUnconfiguredState) && (b > 0))
            {
              USB_DEBUG("Error: USB_StdDevReqClearSetFeature for ep > 0 in unconfigured state!\n");
              UsbDevRequestStallHandshake();
              return;
            }
            // Caution: what shall we do if (b == 0)?  
            UsbDevSelectEndpoint(b);
            if (req.bRequest == USB_StdDevReqCLEAR_FEATURE)
            {
              UsbDevClearStallRequest();
              UsbDevResetEndpoint(b); // should we do an endpoint reset?
              UsbDevResetDataToggleBit();
            }
            else
            {
              UsbDevRequestStallHandshake(); // for endpoint b
            }
            UsbDevSelectEndpoint(0);
            break;
          default:
            USB_DEBUG("Error: USB_StdDevReqClearSetFeature!\n");
            UsbDevRequestStallHandshake();
            return;
        }
        UsbDevSendControlIn(); // send ZLP
        break;
      case USB_StdDevReqSET_ADDRESS: // 2 stages (no data-stage)
        USB_DEBUG("USB_StdDevReqSET_ADDRESS\n");
        if ((LSB(req.wValue) == 0) && (UsbDevConfValue != UsbUnconfiguredState))
        {
          USB_DEBUG("Error: USB_StdDevReqSET_ADDRESS address 0 in configured state!\n");
          UsbDevRequestStallHandshake();
          return;
        }
        ASSERT(req.bmRequestType == 0);
        ASSERT(MSB(req.wValue) == 0);
        ASSERT(req.wIndex == 0);
        ASSERT(req.wLength == 0);
        UsbDevSetAddress(LSB(req.wValue));
        UsbDevSendControlIn(); // send ZLP
        UsbDevWaitTransmitterReady();
        UsbDevEnableAddress();
        break;
      case USB_StdDevReqSET_DESCRIPTOR: // 3 stage-transfer with out data
        USB_DEBUG("Error: USB_StdDevReqSET_DESCRIPTOR currently not supported!\n");
        UsbDevRequestStallHandshake(); // RequestStallHandshake() before UsbDevAcknowledgeSETUP()
        UsbDevAcknowledgeSETUP(); // to ensure that host will not send out data!
        // you may implement this request if you have any idea for what you can use it
        break;
      case USB_StdDevReqGET_CONFIGURATION: // 3 stages with 1 byte IN-data -- not tested yet
        USB_DEBUG("USB_StdDevReqGET_CONFIGURATION\n");
        ASSERT(req.bmRequestType == 128);
        ASSERT(req.wValue == 0);
        ASSERT(req.wIndex == 0);
        ASSERT(req.wLength == 1);
        UsbDevWriteByte(UsbDevConfValue);
        UsbDevSendControlIn();
        WaitZLP_FromHost();
        break;
      case USB_StdDevReqSET_CONFIGURATION: // 2 stages (no data-stage)
        USB_DEBUG("USB_StdDevReqSET_CONFIGURATION: %d\n", LSB(req.wValue));
        ASSERT(req.bmRequestType == 0);
        ASSERT(MSB(req.wValue) == 0);
        ASSERT(req.wIndex == 0);
        ASSERT(req.wLength == 0);
        if (UsbDevSetConfiguration(LSB(req.wValue)))
        {
          UsbDevSelectEndpoint(0); // UsbDevSetConfiguration() may select other ep
          UsbDevSendControlIn(); // send ZLP
        }
        else
        {
          USB_DEBUG("Error: USB_StdDevReqSET_CONFIGURATION unsupported configuration!\n");
          UsbDevSelectEndpoint(0);
          UsbDevRequestStallHandshake();
        }
        break;
      case USB_StdDevReqGET_INTERFACE: // 3 stages with 1 byte IN-data -- not tested yet
        USB_DEBUG("USB_StdDevReqGET_INTERFACE\n");
        if (UsbDevConfValue == UsbUnconfiguredState)
        {
          USB_DEBUG("Error: USB_StdDevReqGET_INTERFACE in unconfigured state!\n");
          UsbDevRequestStallHandshake();
          return;
        }
        ASSERT(req.bmRequestType == 129);
        ASSERT(req.wValue == 0);
        ASSERT(MSB(req.wIndex) == 0);
        ASSERT(req.wLength == 1);
        UsbDevWriteByte(AltSettingOfInterface[(uint8_t) LSB(req.wIndex)]);
        UsbDevSendControlIn();
        WaitZLP_FromHost();
        break;
      case USB_StdDevReqSET_INTERFACE: // 2 stages (no data-stage)
        USB_DEBUG("USB_StdDevReqSET_INTERFACE\n");
        if (UsbDevConfValue == UsbUnconfiguredState)
        {
          USB_DEBUG("Error: USB_StdDevReqSET_INTERFACE in unconfigured state!\n");
          UsbDevRequestStallHandshake();
          return;
        }
        ASSERT(req.bmRequestType == 1);
        ASSERT(MSB(req.wValue) == 0);
        ASSERT(MSB(req.wIndex) == 0);
        ASSERT(req.wLength == 0);
        if (UsbDevSetInterface(UsbDevConfValue, LSB(req.wIndex), LSB(req.wValue)))
        {
          UsbDevSelectEndpoint(0); // UsbDevSetInterface() may select other ep
          UsbDevSendControlIn(); // send ZLP
        }
        else
        {
          UsbDevSelectEndpoint(0);
          USB_DEBUG("Error: USB_StdDevReqSET_INTERFACE alt. setting does not exist!\n");
          UsbDevRequestStallHandshake();
        }
        break;
      case USB_StdDevReqSYNCH_FRAME: // 3 stages with 2 byte IN-data -- not supported
        USB_DEBUG("USB_StdDevReqSYNCH_FRAME not supported!\n");
        UsbDevRequestStallHandshake();
        return;
      case USB_StdDevReqGET_DESCRIPTOR: // 3 stages transfer
        USB_DEBUG("USB_StdDevReqGET_DESCRIPTOR\n");
        ASSERT(req.bmRequestType == 128);
        {
          uint8_t requested;
          uint8_t written = 0;
          if (MSB(req.wLength)) requested = 255; else requested = LSB(req.wLength); // we will never send more than 255 bytes
          switch (MSB(req.wValue))
          {
            case 1: // Device-Descriptor
              ASSERT(req.wIndex == 0);
              ASSERT(LSB(req.wValue) == 0);
              UsbGetDeviceDescriptor(&u.devDes);
              UsbDumpDeviceDescriptor(&u.devDes);
              UsbDevWriteDescriptor(&u.devDes, &written, requested);
              break;
            case 2: // Configuration-Descriptor
              if (!UsbSendDescriptors(LSB(req.wValue), &written, requested))
              {
                USB_DEBUG("Error: USB_StdDevReqGET_DESCRIPTOR: can not sent Configuration-Descriptor!\n");
                UsbDevRequestStallHandshake();
                return;
              }
              break;
            case 3: // String-Descriptor
              UsbGetStringDescriptor(u.strDes, LSB(req.wValue)); // strDes is an array, so no & is necessary!
              UsbDumpStringDescriptor(u.strDes);
              UsbDevWriteDescriptor(u.strDes, &written, requested);
              break;
            default:
              USB_DEBUG("USB_StdDevReqGET_DESCRIPTOR: Unknown type!\n");
              return;
          }
          if UsbDevHasReceivedOUT_Data() // got ZLP from host -- abort! 
          {
            UsbDevClearHasReceivedOUT_Data();
            USB_DEBUG("USB_StdDevReqGET_DESCRIPTOR: Abort from host!\n");
            return;
          }
          if (((written % EP0_FIFO_Size) != 0) || (written < requested))
            UsbDevSendControlIn();
          WaitZLP_FromHost();
        }
        break;
      default:
        USB_DEBUG("Received unsupported UsbStandardRequest!\n");
        UsbDevRequestStallHandshake();
    }
  }
  else
  {
    USB_DEBUG("Unsupported nonstandard request!\n");
    UsbDevRequestStallHandshake();
  }
}

static bool
UsbSendDescriptors(uint8_t cdi, uint8_t *written, uint8_t requested)
{
  union // we can use the same piece of memory for these descriptors
  {
    USB_ConfigurationDescriptor confDes;
    USB_InterfaceDescriptor intDes;
    USB_EndpointDescriptor endDes;
  } u;
  //uint8_t cdi; // configuration descriptor index
  uint8_t idi; // interface descriptor index
  uint8_t as;  // alternate setting of interface
  uint8_t edi; // endpoint descriptor index
  if (UsbGetConfigurationDescriptor(&u.confDes, cdi))
  {
    UsbDumpConfigurationDescriptor(&u.confDes);
    UsbDevWriteDescriptor(&u.confDes, written, requested);
    idi = 0;
    do
    {
      as = 0;
      while (UsbGetInterfaceDescriptor(&u.intDes, cdi, idi, as))
      {
        UsbDumpInterfaceDescriptor(&u.intDes);
        UsbDevWriteDescriptor(&u.intDes, written, requested);
        for (edi = 1; edi < UsbNumEndpointsAT90USB; edi++)
        {
          if (UsbGetEndpointDescriptor(&u.endDes, cdi, idi, as, edi))
          {
            UsbDumpEndpointDescriptor(&u.endDes);
            UsbDevWriteDescriptor(&u.endDes, written, requested);
          }
        }
        as++;
      }
      idi++;
    } while (as > 0);   
    return true;
  }
  else return false;
}

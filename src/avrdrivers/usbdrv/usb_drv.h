// AT90USB/usb_drv.h
// Macros for access to USB registers of Atmels AT90USB microcontrollers
// This file contains low level register stuff as described in
// Atmels AT90USB datasheet 7593D-AVR-07/06
// S. Salewski 21-MAR-2007

#ifndef _USB_DRV_H_
#define _USB_DRV_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include "defines.h" 

#define USB_DEBUG DEBUG
//#define USB_DEBUG(...)

#define UsbNumEndpointsAT90USB	7

#define UsbEP_TypeControl	0
#define UsbEP_TypeIso		1
#define UsbEP_TypeBulk		2
#define UsbEP_TypeInterrupt	3
#define UsbEP_DirOut		0
#define UsbEP_DirControl	0
#define UsbEP_DirIn		1

#define UsbUnconfiguredState	0
#define UsbInterfaceUnconfigured 0xFF
#define UsbNoInterfaceClass	0xFF
#define UsbNoInterfaceSubClass	0xFF
#define UsbNoInterfaceProtokoll	0xFF
#define UsbNoDeviceClass	0xFF
#define UsbNoDeviceSubClass	0xFF
#define UsbNoDeviceProtokoll	0xFF
#define UsbNoDescriptionString	0

extern uint8_t UsbAllocatedEPs;
extern volatile uint8_t UsbStartupFinished;

void UsbDevLaunchDevice(bool lowspeed);
bool UsbDevEP_Setup(uint8_t num, uint8_t type, uint16_t size, uint8_t banks, uint8_t dir);
void UsbInitialReset(void);
void UsbStartPLL(void);
void UsbDevStartDeviceEP0(void);

// A few macros for bit fiddling
#define SetBit(adr, bit)			(adr |=  (1<<bit))
#define ClearBit(adr, bit)			(adr &= ~(1<<bit))
#define BitIsSet(adr, bit)			(adr & (1<<bit))
#define BitIsClear(adr, bit)			(!(adr & (1<<bit)))

// A few simple macros
#define UsbDevWaitStartupFinished()		while (!UsbStartupFinished);
#define UsbOutEndpointAddress(endpointIndex)	(endpointIndex & 15)
#define UsbInEndpointAddress(endpointIndex)	(endpointIndex & 15) | (1<<7)
#define UsbConfigurationValue(confIndex)	(confIndex + 1)			// +1, because 0 indicates unconfigured (addressed) state
//#define UsbMaxPower2mA(mA)			(mA/2)
#define UsbConfDesAttrBusPowered		(1<<7)				// use | to combine these 3 Attributes
#define UsbConfDesAttrSelfPowered		(1<<7) | (1<<6)
#define UsbConfDesAttrRemoteWakeup		(1<<7) | (1<<5)			

// Section and page references refer to release 7593D-AVR-07/06 of
// Atmels Manual for AT90USB devices

// USB general registers, section 21.12.1, page 263 of datasheet
// UHWCON (UsbHardWareCONfiguration)
#define UsbSetDeviceMode()			SetBit(UHWCON, UIMOD)		// select host or device mode manually
#define UsbSetHostMode()			ClearBit(UHWCON, UIMOD)
#define UsbEnableUID_ModeSelection()		SetBit(UHWCON, UIDE)		// enable mode selection by UID pin
#define UsbDisableUID_ModeSelection()		ClearBit(UHWCON, UIDE)
#define UsbEnableUVCON_PinControl()		SetBit(UHWCON, UVCONE)		// enable UVCON pin control, figure 21-7
#define UsbDisableUVCON_PinControl()		ClearBit(UHWCON, UVCON)
#define UsbEnablePadsRegulator()		SetBit(UHWCON, UVREGE)		// USB pads (D+, D-) supply
#define UsbDisablePadsRegulator()		ClearBit(UHWCON, UVREGE)

// USBCON (USB CONfiguration)
#define UsbEnableController()			SetBit(USBCON, USBE)		// USB controller enable
#define UsbDisableController()			ClearBit(USBCON, USBE)		// reset and disable controller
#define UsbIsControllerEnabled()		BitIsSet(USBCON, USBE)	
#define UsbSetHostModeReg()			SetBit(USBCON, HOST)		// select multiplexed controller registers
#define UsbSetDeviceModeReg()			ClearBit(USBCON, HOST)		//
#define UsbFreezeClock()			SetBit(USBCON, FRZCLK)		// reduce power consumption
#define UsbEnableClock()			ClearBit(USBCON, FRZCLK)
#define UsbIsClockFreezed()			BitIsSet(USBCON, FRZCLK)
#define UsbEnableOTG_Pad()			SetBit(USBCON, OTGPADE)		// ??? is this the UID pad?
#define UsbDisableOTG_Pad()			ClearBit(USBCON, OTGPADE)
#define UsbEnableID_TransitionInt()		SetBit(USBCON, IDTE)		// enable ID transition interrupt generation
#define UsbDisableID_TransitionInt()		ClearBit(USBCON, IDTE)
#define UsbEnableVBUS_TransitionInt()		SetBit(USBCON, VBUSTE)		// enable VBUS transition interrupt 
#define UsbDisableVBUS_TransitionInt()		ClearBit(USBCON, VBUSTE)

// USBSTA (USBSTAtus, read only)
#define UsbIsFullSpeedMode()			BitIsSet(USBSTA, SPEED)		// set by hardware if controller is in fullspeed mode,
#define UsbIsLowSpeedMode()			BitIsClear(USBSTA, SPEED)	// use in host mode only, indeterminate in device mode
#define UsbIsUID_PinHigh()			BitIsSet(USBSTA, ID)		// query UID pad/pin
#define UsbIsVBUS_PinHigh()			BitIsSet(USBSTA, VBUS)		// query VBUS pad/pin

// USBINT (USBINTerrupt)
#define UsbIsIDTI_FlagSet()			BitIsSet(USBINT, IDTI)		// set by hardware if ID pin transition detected
#define UsbClearIDTI_Flag()			ClearBit(USBINT, IDTI)		// shall be cleared by software
#define UsbIsVBUSTI_FlagSet()			BitIsSet(USBINT, VBUSTI)	// set by hardware if transition on VBUS pad is detected
#define UsbClearVBUSTI_Flag()			ClearBit(USBINT, VBUSTI)	// shall be cleared by software

// OTGCON (OnTheGoCONfiguration)
// to do...

// OTGTCON (OnTheGoTimerCONfiguration)
// to do...

// OTGIEN (OnTheGoInterruptEnable)
// to do...

// OTGINT (OnTheGoINTerrupt)
// to do...

// USB device general registers, section 22.19.1, page  281
// UDCON (Usb Device CONfiguration)
#define UsbDevSelectLowSpeed()			SetBit(UDCON, LSM)		// Page 258, figure 21-14
#define UsbDevSelectFullSpeed()			ClearBit(UDCON, LSM)
#define UsbDevIsLowSpeedSelected()		BitIsSet(UDCON, LSM)
#define UsbDevInitiateRemoteWakeup()		SetBit(UDCON, RMWKUP)		// cleared by hardware, see section 22.11, page 273 
#define UsbDevIsRemoteWakeupPending()		BitIsSet(UDCON, RMWKUP)
#define UsbDevDetach()				SetBit(UDCON, DETACH)		// disconnect internal pull-up on D+ or D-
#define UsbDevAttach()				ClearBit(UDCON, DETACH)		// connect internal pull-up on D+ or D-
#define UsbDevIsDetached()			BitIsSet(UDCON, DETACH)
#define UsbDevIsAttached()			BitIsClear(UDCON, DETACH)

// UDINT (Usb Device INTerrupt)
#define UsbDevIsUpstreamResumeFlagSet()	BitIsSet(UDINT, UPRSMI)		// set by hardware
#define UsbDevClearUpstreamResumeFlag()	ClearBit(UDINT, UPRSMI)		// shall be cleared by software
#define UsbDevIsEndOfResumeFlagSet()		BitIsSet(UDINT, EORSMI)		// set by hardware if host sends "end of resume"
#define UsbDevClearEndOfResumeFlag()		ClearBit(UDINT, EORSMI)		// shall be cleared by software
#define UsbDevIsWakeupCPU_FlagSet()		BitIsSet(UDINT, WAKEUPI)	// set by hardware when USB controller is reactivated
#define UsbDevClearWakeupCPU_Flag()		ClearBit(UDINT, WAKEUPI)	// shall be cleared by software after enabling USB clock inputs
#define UsbDevIsEndOfResetFlagSet()		BitIsSet(UDINT, EORSTI)		// set by hardware when USB controller has detected "End Of Reset"
#define UsbDevClearEndOfResetFlag()		ClearBit(UDINT, EORSTI)		// shall be cleared by software
#define UsbDevIsStartOfFrameFlagSet()		BitIsSet(UDINT, SOFI)		// set by hardware when an USB "Start Of Frame" has been detected
#define UsbDevClearStartOfFrameFlag()		ClearBit(UDINT, SOFI)
#define UsbDevIsSuspendFlagSet()		BitIsSet(UDINT, SUSPI)		// set by hardware when USB bus is idle
#define UsbDevClearSuspendFlag()		ClearBit(UDINT, SUSPI)		// shall be cleared by software

// UDIEN (Usb Device Interrupt ENable)
#define UsbDevEnableUpstreamResumeInt()	SetBit(UDIEN, UPRSME)
#define UsbDevDisableUpstreamResumeInt()	ClearBit(UDIEN, UPRSME)
#define UsbDevEnableEndOfResumeInt()		SetBit(UDIEN, EORSME)
#define UsbDevDisableEndOfResumeInt()		Clear(UDIEN, EORSME)
#define UsbDevEnableWakeupCPU_Int()		SetBit(UDIEN, WAKEUPE)
#define UsbDevDisableWakeupCPU_Int()		ClearBit(UDIEN, WAKEUPE)
#define UsbDevEnableEndOfResetInt()		SetBit(UDIEN, EORSTE)
#define UsbDevDisableEndOfResetInt()		ClearBit(UDIEN, EORSTE)
#define UsbDevEnableStartOfFrameInt()		SetBit(UDIEN, SOFE)
#define UsbDevDisableStartOfFrameInt()		ClearBit(UDIEN, SOFE)
#define UsbDevEnableSuspendInt()		SetBit(UDIEN, SUSPE)
#define UsbDevDisableSuspendInt()		ClearBit(UDIEN, SUSPE)

// UDADDR (Usb Device ADDRess)
#define UsbDevEnableAddress()			SetBit(UDADDR, ADDEN)		// cleared by hardware
#define UsbDevSetAddress(adr)			UDADDR = (adr & 127)		// set by software

// UDFNUMH (Usb Device Frame NUMber High)
#define UsbDevGetFrameNumberHigh(num)		num = UDFNUMH			// 3 MSB of 11-bits frame number; set by hardware

// UDFNUML (Usb Device Frame NUMber Low)
#define UsbDevGetFrameNumberLow(num)		num = UDFNUML			// 8 LSB of 11-bits frame number; set by hardware

// UDMFN
#define UsbDevIsFrameNumberCRC_Error()		BitIsSet(UDMFN, FNCERR)		// set by hardware if CRC error of framenumber

// USB device endpoint registers, section 22.19.2 page 284
// UENUM (Usb Endpoint NUMber)
#define UsbDevSelectEndpoint(num)		UENUM = (num & 7)		// num == 0, 1, ..., 6

// UERST (Usb Endpoint ReSeT)
#define UsbDevResetEndpoints(mask)		UERST = mask & 127; UERST = 0	// reset selected endpoints; is UERST = 0 necessary?
#define UsbDevResetEndpoint(num)		UERST |= (1<<num); UERST = 0	// reset this endpoint

// UECONX (Usb Endpoint CONfiguration X)
#define UsbDevRequestStallHandshake()		SetBit(UECONX, STALLRQ)		// request a STALL answer to the host for next handshake
#define UsbDevClearStallRequest()		SetBit(UECONX,STALLRQC)		// disable STALL handshake mechanism
#define UsbDevIsEndpointStalled()		BitIsSet(UECONX, STALLRQ)	// Caution: This bit is marked write-only in datasheet
#define UsbDevResetDataToggleBit()		SetBit(UECONX, RSTDT)		// next packet is data0
#define UsbDevEnableEndpoint()			SetBit(UECONX, EPEN)		// enable endpoint according to device configuration
#define UsbDevDisableEndpoint()			ClearBit(UECONX, EPEN)
#define UsbDevIsEndpointEnabled()		BitIsSet(UECONX, EPEN)

// UECFG0X (Usb Endpoint ConFiGuration 0X)
#define UsbDevSetEndpointTypeControl()		UECFG0X &= ~((1<<EPTYPE1)|(1<<EPTYPE0))
#define UsbDevSetEndpointTypeIso()		UECFG0X  = (UECFG0X & ~(1<<EPTYPE1)) | (1<<EPTYPE0) 
#define UsbDevSetEndpointTypeBulk()		UECFG0X  = (UECFG0X & ~(1<<EPTYPE0)) | (1<<EPTYPE1) 
#define UsbDevSetEndpointTypeInt()		UECFG0X |= ((1<<EPTYPE1)|(1<<EPTYPE0))
#define UsbDevSetEnpointDirectionIn()		SetBit(UECFG0X, EPDIR)
#define UsbDevSetEnpointDirectionOut()		ClearBit(UECFG0X, EPDIR)
#define UsbDevSetEnpointDirectionControl()	ClearBit(UECFG0X, EPDIR)

// UECFG1X (Usb Endpoint ConFiGuration 1X)
// here we access bits direct, without using bitnames
#define UsbDevSetEndpointSize8()		UECFG1X = (UECFG1X & (4+8))	// preserve bank, clear ALLOC, set size
#define UsbDevSetEndpointSize16()		UECFG1X = (UECFG1X & (4+8)) | (1<<4)
#define UsbDevSetEndpointSize32()		UECFG1X = (UECFG1X & (4+8)) | (2<<4)
#define UsbDevSetEndpointSize64()		UECFG1X = (UECFG1X & (4+8)) | (3<<4)
#define UsbDevSetEndpointSize128()		UECFG1X = (UECFG1X & (4+8)) | (4<<4)
#define UsbDevSetEndpointSize256()		UECFG1X = (UECFG1X & (4+8)) | (5<<4)
#define UsbDevSetEndpointSize512()		UECFG1X = (UECFG1X & (4+8)) | (6<<4)
#define UsbDevSetEndpointOneBank()		UECFG1X = (UECFG1X & (7<<4))	// preserve size, clear ALLOC, set bank
#define UsbDevSetEndpointDoubleBank()		UECFG1X = (UECFG1X & (7<<4)) | (1<<EPBK0)
#define UsbDevSetEndpointAllocBit()		SetBit(UECFG1X, ALLOC)		// allocate endpoint memory
#define UsbDevClearEndpointAllocBit()		ClearBit(UECFG1X, ALLOC)	// free endpoint memory

// UESTA0X (Usb Endpoint STAtus 0X)
#define UsbDevIsConfigurationOk()		BitIsSet(UESTA0X, CFGOK)	// updated when ALLOC bit is set
#define UsbDevIsOverflowErrorFlagSet()		BitIsSet(UESTA0X, OVERFL)	// indicates overflow in isochronous endpoint
#define UsbDevClearOverflowErrorFlag()		ClearBit(UESTA0X, OVERFL)	// shall be cleared by software
#define UsbDevIsUnderflowErrorFlagSet()		BitIsSet(UESTA0X, UNDERFL)	// indicates underflow in isochronous endpoint
#define UsbDevClearUnderflowErrorFlag()		ClearBit(UESTA0X, UNDERFL)	// shall be cleared by software
#define UsbDevZeroLengthPackedSeen()		BitIsSet(UESTA0X, ZLPSEEN)
#define UsbDevClearZeroLengthPackedFlag()	ClearBit(UESTA0X, ZLPSEEN)	// shall be cleared by software
#define UsbDevIsData0()				UESTA0X & ((1<<DTSEQ0) | (1<<DTSEQ1)) == 0
#define UsbDevIsData1()				UESTA0X & ((1<<DTSEQ0) | (1<<DTSEQ1)) == (1<<DTSEQ0)
#define UsbDevGetNumberOfBusyBanks()		(UESTA0X & 3)			// if 0 then all banks are free, if 1 then 1 bank is busy...

// UESTA1X (Usb Endpoint STAtus 1X)
#define UsbDevIsControlDirectionIn()		BitIsSet(UESTA1X, CTRLDIR) 	// direction of next packed after SETUP packed
#define UsbDevIsControlDirectionOut()		BitIsClear(UESTA1X, CTRLDIR)	// set and cleared by hardware
#define UsbDevGetCurrentBank()			(UESTA1X & 3)			// set by hardware, number of current bank, 0 or 1

// UEINTX (Usb Endpoint INTerrupt X)
#define UsbDevIsFifoControllBitSet()		BitIsSet(UEINTX, FIFOCON)	// see page 289
#define UsbDevClearFifoControllBit()		ClearBit(UEINTX, FIFOCON)
#define UsbDevSendInData()			ClearBit(UEINTX, FIFOCON)	// see section 22.15, page 277
#define UsbDevNAK_ResponseSendToInRequest()	BitIsSet(UEINTX, NAKINI)
#define UsbDevClearNAK_ResponseInBit()		ClearBit(UEINTX, NAKINI)	// shall be cleared by software
//#define UsbDevReadWriteAllowed()		BitIsSet(UEINTX, RWAL)		// set by hardware, don't use for control endpoint
#define UsbDevReadAllowed()			BitIsSet(UEINTX, RWAL)
#define UsbDevWriteAllowed()			BitIsSet(UEINTX, RWAL)
#define UsbDevNAK_ResponseSendToOutRequest()	BitIsSet(UEINTX, NAKOUTI)
#define UsbDevClearNAK_ResponseOutBit()		ClearBit(UEINTX, NAKOUTI)	// shall be cleared by software
#define UsbDevHasReceivedSETUP()		BitIsSet(UEINTX, RXSTPI)	// set by hardware if current bank contains a valid SETUP packet
//#define UsbDevClearHasReceivedSETUP()		ClearBit(UEINTX, RXSTPI)	// shall be cleared by software
#define UsbDevAcknowledgeSETUP()		ClearBit(UEINTX, RXSTPI)	// acknowledge request and clear fifo, see section 22.13
#define UsbDevHasReceivedOUT_Data()		BitIsSet(UEINTX, RXOUTI)	// set by hardware if current bank contains a new packed
#define UsbDevClearHasReceivedOUT_Data()	ClearBit(UEINTX, RXOUTI)	// shall be cleared by software
#define UsbDevKillLastWrittenBank()		SetBit(UEINTX, RXOUTI)		// see page 278 for abort operation
#define UsbDevSTALLHandshakeSend()		BitIsSet(UEINTX, STALLEDI) 	// STALL send or CRC error in OUT isochronous endpoint
#define UsbDevTransmitterReady()		BitIsSet(UEINTX, TXINI)		// current bank is free and can be filled
#define UsbDevWaitTransmitterReady()		while (!(UEINTX & (1<<TXINI)));
#define UsbDevClearTransmitterReady()		ClearBit(UEINTX, TXINI)		// shall be cleared by software
#define UsbDevSendControlIn()			ClearBit(UEINTX, TXINI)		// see section 22.13, page 274 (control endpoint management)
#define UsbDevAcknowledgeInBankFreeInt()	ClearBit(UEINTX, TXINI)		// TXINI is set by hardware if in bank becomes free, see section 22.15

// UEIENX (Usb Endpoint Interupt ENable X)
#define UsbDevEnableFlowErrorInt()		SetBit(UEIENX, FLERRE)	
#define UsbDevDisableFlowErrorInt()		ClearBit(UEIENX, FLERRE)
#define UsbDevEnableNAK_IN_Int()		SetBit(UEIENX, NAKINE)
#define UsbDevDisableNAK_IN_Int()		ClearBit(UEIENX, NAKINE)
#define UsbDevEnableNAK_OUT_Int()		SetBit(UEIENX, NAKOUTE)
#define UsbDevDisableNAK_OUT_Int()		ClearBit(UEIENX, NAKOUTE)
#define UsbDevEnableReceivedSETUP_Int()		SetBit(UEIENX, RXSTPE)
#define UsbDevDisableReceivedSETUP_Int()	ClearBit(UEIENX, RXSTPE)
#define UsbDevEnableReceivedOUT_DATA_Int()	SetBit(UEIENX, RXOUTE)
#define UsbDevDisableReceivedOUT_DATA_Int()	ClearBit(UEIENX, RXOUTE)
#define UsbDevEnableSTALLED_Int()		SetBit(UEIENX, STALLEDE)
#define UsbDevDisableSTALLED_Int()		ClearBit(UEIENX, STALLEDE)
#define UsbDevEnableTransmitterReadyInt()	SetBit(UEIENX, TXINE)
#define UsbDevDisableTransmitterReadyInt()	ClearBit(UEIENX, TXINE)

// UEDATX (Usb Endpoint DATa X)
#define UsbDevReadByte()			UEDATX				// read byte from endpoint FIFO selected by EPNUM
#define UsbDevWriteByte(byte)			UEDATX = byte			// write byte to endpoint FIFO selected by EPNUM

// UEBCHX (Usb Endpoint Byte Count High X)
#define UsbDevGetByteCountHigh()		UEBCHX				// 3 MSB of the byte count of the FIFO endpoint

// UEBCLX (Usb Endpoint Byte Count Low X)
#define UsbDevGetByteCountLow()			UEBCLX				// 8 LSB of the byte count of the FIFO endpoint

// UEINT (Usb Endpoint INTerrupt)
#define UsbDevGetEndpointIntBits()		UEINT				// set by hardware when interrupt is triggered, cleared if int source is served

// PLL clock for USB interface, section 6.11, page 50
// PLLCSR (PLL Control and Status Register)
// set PLL prescaler according to XTAL crystal frequency
//#define UsbXTALFrequencyIs2MHz()		PLLCSR = (PLLCSR & ~(15<<1))
//#define UsbXTALFrequencyIs4MHz()		PLLCSR = (PLLCSR & ~(15<<1)) | (1<<2)
//#define UsbXTALFrequencyIs6MHz()		PLLCSR = (PLLCSR & ~(15<<1)) | (2<<2)
//#define UsbXTALFrequencyIs8MHz()		PLLCSR = (PLLCSR & ~(15<<1)) | (3<<2)
//#define UsbXTALFrequencyIs12MHz()		PLLCSR = (PLLCSR & ~(15<<1)) | (4<<2)
//#define UsbXTALFrequencyIs16MHz()		PLLCSR = (PLLCSR & ~(15<<1)) | (5<<2)
#if (F_XTAL == 2000000)
#define _pre_ 0
#elif (F_XTAL == 4000000)
#define _pre_ 1
#elif (F_XTAL == 6000000)
#define _pre_ 2
#elif (F_XTAL == 8000000)
#define _pre_ 3
#elif (F_XTAL == 12000000)
#define _pre_ 4
#elif (F_XTAL == 16000000)
#define _pre_ 5
#else
  #error "XTAL-Frequency has to be 2, 4, 6, 8, 12 or 16 MHz for USB devices!"
#endif
#define UsbSetPLL_XTAL_Frequency()		PLLCSR = (_pre_<<2)
#define UsbEnablePLL()				SetBit(PLLCSR, PLLE)
#define UsbDisablePLL()				ClearBit(PLLCSR, PLLE)
#define UsbIsPLL_Locked()			BitIsSet(PLLCSR, PLOCK)
#define UsbWaitPLL_Locked()			while (!(PLLCSR & (1<<PLOCK)));

#endif

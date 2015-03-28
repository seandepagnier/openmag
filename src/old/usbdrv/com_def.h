// AT90USB/com_def.h
// Common defines for firmware and PC program
// S. Salewski, 23-MAR-2007

#ifndef _COM_DEF_H_
#define _COM_DEF_H_

//#define MyUSB_VendorID 0x16c0 // code
//#define MyUSB_ProductID 0x06dd // value

#define MyUSB_VendorID 0x16c0 // code
#define MyUSB_ProductID 0x06dd // value

#define USB_VendorRequestCode (1<<6)

// Endpoint 1, used for transferring status information:
// Bulk OUT, 64 byte single bank FIFO
#define EP1_FIFO_Size 64

// Endpoint 2, used for transferring DAQ data:
// Bulk IN, 64 byte dual bank
#define EP2_FIFO_Size 64

#endif

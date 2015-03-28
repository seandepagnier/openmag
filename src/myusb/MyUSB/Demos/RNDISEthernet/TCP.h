/*
             MyUSB Library
     Copyright (C) Dean Camera, 2008.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2008  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, and distribute this software
  and its documentation for any purpose and without fee is hereby
  granted, provided that the above copyright notice appear in all
  copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#ifndef _TCP_H_
#define _TCP_H_

	/* Includes: */
		#include <avr/io.h>
		#include <string.h>
		#include <stdbool.h>
		
		#include <MyUSB/Scheduler/Scheduler.h>
		
		#include "EthernetProtocols.h"
		#include "Ethernet.h"
		#include "ProtocolDecoders.h"
		
	/* Macros: */
		#define MAX_OPEN_TCP_PORTS              2
		#define MAX_TCP_CONNECTIONS             2
		#define TCP_WINDOW_SIZE                 1024
		
		#define TCP_PORT_TELNET                 SwapEndian_16(23)
		#define TCP_PORT_HTTP                   SwapEndian_16(80)
		
		#define TCP_PACKETDIR_IN                false
		#define TCP_PACKETDIR_OUT               true
		
		#define TCP_FLAG_CWR                    (1 << 7)
		#define TCP_FLAG_ECE                    (1 << 6)
		#define TCP_FLAG_URG                    (1 << 5)
		#define TCP_FLAG_ACK                    (1 << 4)
		#define TCP_FLAG_PSH                    (1 << 3)
		#define TCP_FLAG_RST                    (1 << 2)
		#define TCP_FLAG_SYN                    (1 << 1)
		#define TCP_FLAG_FIN                    (1 << 0)
		
		#define TCP_APP_HAS_RECEIVED_PACKET(Buffer)  (Buffer->Ready && (Buffer->Direction == TCP_PACKETDIR_IN))
		#define TCP_APP_HAVE_CAPTURED_BUFFER(Buffer) (!(Buffer->Ready) && Buffer->InUse && \
		                                              (Buffer->Direction == TCP_PACKETDIR_OUT))
		#define TCP_APP_CAN_CAPTURE_BUFFER(Buffer)   Buffer->InUse
		#define TCP_APP_SEND_BUFFER(Buffer, Len)     MACROS{ Buffer->Direction = TCP_PACKETDIR_OUT; Buffer->Length = Len; Buffer->Ready = true; }MACROE
		#define TCP_APP_CLEAR_BUFFER(Buffer)         MACROS{ Buffer->Ready = false; Buffer->Length = 0; }MACROE
		#define TCP_APP_CAPTURE_BUFFER(Buffer)       MACROS{ Buffer->Direction = TCP_PACKETDIR_OUT; Buffer->InUse = true; }MACROE
		#define TCP_APP_RELEASE_BUFFER(Buffer)       MACROS{ Buffer->InUse = false; }MACROE

	/* Enums: */
		enum TCP_PortStates_t
		{
			TCP_Port_Closed            = 0,
			TCP_Port_Open              = 1,
		};
	
		enum TCP_ConnectionStates_t
		{
			TCP_Connection_Listen      = 0,
			TCP_Connection_SYNSent     = 1,
			TCP_Connection_SYNReceived = 2,
			TCP_Connection_Established = 3,
			TCP_Connection_FINWait1    = 4,
			TCP_Connection_FINWait2    = 5,
			TCP_Connection_CloseWait   = 6,
			TCP_Connection_Closing     = 7,
			TCP_Connection_LastACK     = 8,
			TCP_Connection_TimeWait    = 9,
			TCP_Connection_Closed      = 10,			
		};
	
	/* Type Defines: */
		typedef struct
		{
			uint16_t               Length;
			uint8_t                Data[TCP_WINDOW_SIZE];
			bool                   Direction;
			bool                   Ready;
			bool                   InUse;		
		} TCP_ConnectionBuffer_t;

		typedef struct
		{
			uint32_t               SequenceNumberIn;			
			uint32_t               SequenceNumberOut;
			TCP_ConnectionBuffer_t Buffer;
		} TCP_ConnectionInfo_t;

		typedef struct
		{
			uint16_t               Port;
			uint8_t                State;
			void                   (*ApplicationHandler) (TCP_ConnectionBuffer_t* Buffer);
		} TCP_PortState_t;
	
		typedef struct
		{
			uint16_t               Port;
			uint16_t               RemotePort;
			IP_Address_t           RemoteAddress;
			TCP_ConnectionInfo_t   Info;
			uint8_t                State;
		} TCP_ConnectionState_t;

		typedef struct
		{
			uint16_t               SourcePort;
			uint16_t               DestinationPort;
			
			uint32_t               SequenceNumber;
			uint32_t               AcknowledgmentNumber;
			
			unsigned int           Reserved : 4;
			unsigned int           DataOffset : 4;
			uint8_t                Flags;
			uint16_t               WindowSize;
			
			uint16_t               Checksum;
			uint16_t               UrgentPointer;
		} TCP_Header_t;

	/* Tasks: */
		TASK(TCP_Task);
		
	/* External Variables: */
		TCP_PortState_t PortStateTable[MAX_OPEN_TCP_PORTS];

	/* Function Prototypes: */
		void                  TCP_Init(void);
		bool                  TCP_SetPortState(uint16_t Port, uint8_t State, void (*Handler)(TCP_ConnectionBuffer_t*));
		uint8_t               TCP_GetPortState(uint16_t Port);
		bool                  TCP_SetConnectionState(uint16_t Port, IP_Address_t RemoteAddress, uint16_t RemotePort, uint8_t State);
		uint8_t               TCP_GetConnectionState(uint16_t Port, IP_Address_t RemoteAddress, uint16_t RemotePort);
		TCP_ConnectionInfo_t* TCP_GetConnectionInfo(uint16_t Port, IP_Address_t RemoteAddress, uint16_t RemotePort);
		int16_t               TCP_ProcessTCPPacket(void* IPHeaderInStart, void* TCPHeaderInStart, void* TCPHeaderOutStart);

		#if defined(INCLUDE_FROM_TCP_C)
			static uint16_t TCP_Checksum16(void* TCPHeaderOutStart, IP_Address_t SourceAddress,
										   IP_Address_t DestinationAddress, uint16_t TCPOutSize);
		#endif

#endif

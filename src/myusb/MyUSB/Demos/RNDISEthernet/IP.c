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

#include "IP.h"

int16_t IP_ProcessIPPacket(void* InDataStart, void* OutDataStart)
{
	DecodeIPHeader(InDataStart);

	IP_Header_t* IPHeaderIN  = (IP_Header_t*)InDataStart;
	IP_Header_t* IPHeaderOUT = (IP_Header_t*)OutDataStart;

	/* Header length is specified in number of longs in the packet header, convert to bytes */
	uint16_t     HeaderLengthBytes = (IPHeaderIN->HeaderLength * sizeof(uint32_t));

	int16_t      RetSize     = NO_RESPONSE;

	FrameIN.FrameLength -= HeaderLengthBytes;

	/* Check to ensure the IP packet is addressed to the virtual webserver's IP */
	if (!(IP_COMPARE(&IPHeaderIN->DestinationAddress, &ServerIPAddress)))
	  return NO_RESPONSE;
	
	/* Pass off the IP payload to the appropriate protocol processing routine */
	switch (IPHeaderIN->Protocol)
	{
		case PROTOCOL_ICMP:
			RetSize = ICMP_ProcessICMPPacket(&((uint8_t*)InDataStart)[HeaderLengthBytes],
			                                 &((uint8_t*)OutDataStart)[sizeof(IP_Header_t)]);
			break;
		case PROTOCOL_TCP:
			RetSize = TCP_ProcessTCPPacket(InDataStart,
			                               &((uint8_t*)InDataStart)[HeaderLengthBytes],
			                               &((uint8_t*)OutDataStart)[sizeof(IP_Header_t)]);		
			break;
	}
	
	/* Check to see if the protocol processing routine has filled out a response */
	if (RetSize > 0)
	{
		/* Fill out the response IP packet header */
		IPHeaderOUT->TotalLength        = SwapEndian_16(sizeof(IP_Header_t) + RetSize);
		IPHeaderOUT->TypeOfService      = 0;
		IPHeaderOUT->HeaderLength       = (sizeof(IP_Header_t) / sizeof(uint32_t));
		IPHeaderOUT->Version            = 4;
		IPHeaderOUT->Flags              = 0;
		IPHeaderOUT->FragmentOffset     = 0;
		IPHeaderOUT->Identification     = 0;
		IPHeaderOUT->HeaderChecksum     = 0;
		IPHeaderOUT->Protocol           = IPHeaderIN->Protocol;
		IPHeaderOUT->TTL                = DEFAULT_TTL;
		IPHeaderOUT->SourceAddress      = IPHeaderIN->DestinationAddress;
		IPHeaderOUT->DestinationAddress = IPHeaderIN->SourceAddress;
		
		IPHeaderOUT->HeaderChecksum     = Ethernet_Checksum16(IPHeaderOUT, sizeof(IP_Header_t));
						
		/* Return the size of the response so far */
		return (sizeof(IP_Header_t) + RetSize);
	}
	
	return RetSize;
}

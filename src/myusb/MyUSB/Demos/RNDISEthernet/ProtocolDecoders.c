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

/* Protocol decoders for Ethernet, TCP, IP, ICMP and ARP. Each of these routines
   accepts a header to the appropriate protocol and prints out pertient information
   on the packet through the serial port.
   
   To disable printing of a specific protocol, define the token NO_DECODE_{Protocol}
   in the project makefile, and pass it to the compiler using the -D switch.
*/

#include "ProtocolDecoders.h"

void DecodeEthernetFrameHeader(void* InDataStart)
{
	#if !defined(NO_DECODE_ETHERNET)
	Ethernet_Frame_Header_t* FrameHeader = (Ethernet_Frame_Header_t*)InDataStart;
	
	printf_P(PSTR("\r\n"));
	
	printf_P(PSTR("  ETHERNET\r\n"));
	printf_P(PSTR("  + Frame Size: %u\r\n"), FrameIN.FrameLength);

	if (!(MAC_COMPARE(&FrameHeader->Destination, &ServerMACAddress)) &&
	    !(MAC_COMPARE(&FrameHeader->Destination, &BroadcastMACAddress)))
	{
		printf_P(PSTR("  + NOT ADDRESSED TO DEVICE\r\n"));
		return;
	}

	printf_P(PSTR("  + MAC Source : %02X:%02X:%02X:%02X:%02X:%02X\r\n"), FrameHeader->Source.Octets[0],
	                                                                     FrameHeader->Source.Octets[1],
	                                                                     FrameHeader->Source.Octets[2],
	                                                                     FrameHeader->Source.Octets[3],
	                                                                     FrameHeader->Source.Octets[4],
	                                                                     FrameHeader->Source.Octets[5]);

	printf_P(PSTR("  + MAC Dest: %02X:%02X:%02X:%02X:%02X:%02X\r\n"),    FrameHeader->Destination.Octets[0],
	                                                                     FrameHeader->Destination.Octets[1],
	                                                                     FrameHeader->Destination.Octets[2],
	                                                                     FrameHeader->Destination.Octets[3],
	                                                                     FrameHeader->Destination.Octets[4],
	                                                                     FrameHeader->Destination.Octets[5]);

	if (SwapEndian_16(FrameIN.FrameLength) > ETHERNET_VER2_MINSIZE)
	  printf_P(PSTR("  + Protocol: 0x%04x\r\n"), SwapEndian_16(FrameHeader->EtherType));
	else
	  printf_P(PSTR("  + Protocol: UNKNOWN E1\r\n"));
	#endif
}

void DecodeARPHeader(void* InDataStart)
{
	#if !defined(NO_DECODE_ARP)
	Ethernet_ARP_Header_t* ARPHeader = (Ethernet_ARP_Header_t*)InDataStart;	

	printf_P(PSTR("   \\\r\n    ARP\r\n"));

	if (!(IP_COMPARE(&ARPHeader->TPA, &ServerIPAddress)) &&
	    !(MAC_COMPARE(&ARPHeader->THA, &ServerMACAddress)))
	{
		printf_P(PSTR("    + NOT ADDRESSED TO DEVICE\r\n"));
		return;		
	}

	printf_P(PSTR("    + Protocol: %x\r\n"), SwapEndian_16(ARPHeader->ProtocolType));
	printf_P(PSTR("    + Operation: %u\r\n"), SwapEndian_16(ARPHeader->Operation));
	
	if (SwapEndian_16(ARPHeader->ProtocolType) == ETHERTYPE_IPV4)
	{
		printf_P(PSTR("    + SHA MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n"), ARPHeader->SHA.Octets[0],
		                                                                   ARPHeader->SHA.Octets[1],
		                                                                   ARPHeader->SHA.Octets[2],
		                                                                   ARPHeader->SHA.Octets[3],
		                                                                   ARPHeader->SHA.Octets[4],
		                                                                   ARPHeader->SHA.Octets[5]);

		printf_P(PSTR("    + SPA IP: %u.%u.%u.%u\r\n"), ARPHeader->SPA.Octets[0],
		                                                ARPHeader->SPA.Octets[1],
		                                                ARPHeader->SPA.Octets[2],
		                                                ARPHeader->SPA.Octets[3]);

		printf_P(PSTR("    + THA MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n"), ARPHeader->THA.Octets[0],
		                                                                   ARPHeader->THA.Octets[1],
		                                                                   ARPHeader->THA.Octets[2],
		                                                                   ARPHeader->THA.Octets[3],
		                                                                   ARPHeader->THA.Octets[4],
		                                                                   ARPHeader->THA.Octets[5]);

		printf_P(PSTR("    + TPA IP: %u.%u.%u.%u\r\n"), ARPHeader->TPA.Octets[0],
		                                                ARPHeader->TPA.Octets[1],
		                                                ARPHeader->TPA.Octets[2],
		                                                ARPHeader->TPA.Octets[3]);
	}
	#endif
}

void DecodeIPHeader(void* InDataStart)
{
	#if !defined(NO_DECODE_IP)
	Ethernet_IP_Header_t* IPHeader  = (Ethernet_IP_Header_t*)InDataStart;

	uint16_t              HeaderLengthBytes = (IPHeader->HeaderLength * sizeof(uint32_t));

	printf_P(PSTR("   \\\r\n    IP\r\n"));

	if (!(IP_COMPARE(&IPHeader->DestinationAddress, &ServerIPAddress)))
	{
		printf_P(PSTR("    + NOT ADDRESSED TO DEVICE\r\n"));
		return;
	}

	printf_P(PSTR("    + Header Length: %u Bytes\r\n"), HeaderLengthBytes);
	printf_P(PSTR("    + Packet Version: %u\r\n"), IPHeader->Version);
	printf_P(PSTR("    + Total Length: %u\r\n"), SwapEndian_16(IPHeader->TotalLength));
	
	printf_P(PSTR("    + Protocol: %u\r\n"), IPHeader->Protocol);
	printf_P(PSTR("    + TTL: %u\r\n"), IPHeader->TTL);
	
	printf_P(PSTR("    + IP Src: %u.%u.%u.%u\r\n"), IPHeader->SourceAddress.Octets[0],
	                                                IPHeader->SourceAddress.Octets[1],
	                                                IPHeader->SourceAddress.Octets[2],
	                                                IPHeader->SourceAddress.Octets[3]);	

	printf_P(PSTR("    + IP Dst: %u.%u.%u.%u\r\n"), IPHeader->DestinationAddress.Octets[0],
	                                                IPHeader->DestinationAddress.Octets[1],
	                                                IPHeader->DestinationAddress.Octets[2],
	                                                IPHeader->DestinationAddress.Octets[3]);
	#endif
}

void DecodeICMPHeader(void* InDataStart)
{
	#if !defined(NO_DECODE_ICMP)
	Ethernet_ICMP_Header_t* ICMPHeader  = (Ethernet_ICMP_Header_t*)InDataStart;

	printf_P(PSTR("    \\\r\n     ICMP\r\n"));

	printf_P(PSTR("     + Type: %u\r\n"), ICMPHeader->Type);
	printf_P(PSTR("     + Code: %u\r\n"), ICMPHeader->Code);
	#endif
}

void DecodeTCPHeader(void* InDataStart)
{
	#if !defined(NO_DECODE_TCP)
	Ethernet_TCP_Header_t* TCPHeader  = (Ethernet_TCP_Header_t*)InDataStart;

	uint16_t               HeaderLengthBytes = (TCPHeader->DataOffset * sizeof(uint32_t));

	printf_P(PSTR("    \\\r\n     TCP\r\n");

	printf_P(PSTR("     + Header Length: %u Bytes\r\n"), HeaderLengthBytes);

	printf_P(PSTR("     + Source Port: %u\r\n"), SwapEndian_16(TCPHeader->SourcePort));
	printf_P(PSTR("     + Destination Port: %u\r\n"), SwapEndian_16(TCPHeader->DestinationPort));

	printf_P(PSTR("     + Sequence Number: %lu\r\n"), SwapEndian_32(TCPHeader->SequenceNumber));
	printf_P(PSTR("     + Acknowledgment Number: %lu\r\n"), SwapEndian_32(TCPHeader->AcknowledgmentNumber));
	
	printf_P(PSTR("     + Flags: 0x%02X\r\n"), TCPHeader->Flags);
	
	if (TCP_GetPortState(TCPHeader->DestinationPort) == TCP_Port_Closed)
	  printf_P(PSTR("     + NOT LISTENING ON DESTINATION PORT\r\n"));
	#endif
}

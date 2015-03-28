/*
             MyUSB Library
     Copyright (C) Dean Camera, 2007.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com

 Released under the LGPL Licence, Version 3
*/

#ifndef __LEDS_USBKEY_H__
#define __LEDS_USBKEY_H__

	/* Includes: */
		#include <avr/io.h>

		#include "../../MyUSB/Common/Common.h"

	/* Preprocessor Checks: */
		#if !defined(INCLUDE_FROM_LEDS_H)
			#error Do not include this file directly. Include MyUSB/Drivers/Board/LEDS.h instead.
		#endif

	/* Private Interface - For use in library only: */
		/* Inline Functions: */
                        #define LEDs_Init()
			#define LEDs_TurnOnLEDs(...)
                        #define LEDs_TurnOffLEDs(...)
			#define LEDs_SetAllLEDs(...)
                        #define LEDs_ChangeLEDs(...)
			#define LEDs_GetLEDs()
			#define LEDs_GetLEDs()
#endif

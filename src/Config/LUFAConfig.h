/*
  Copyright 2014  Luc Hondareyte <luc.hondareyte@laposte.net>

  Based on LUFA MIDI Demo : 
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#ifndef _LUFA_CONFIG_H_
#define _LUFA_CONFIG_H_

	#if (ARCH == ARCH_AVR8)

		/* Non-USB Related Configuration Tokens: */

		/* USB Class Driver Related Tokens: */

		/* General USB Driver Related Tokens: */
		#define USE_STATIC_OPTIONS               (USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)
		#define USB_DEVICE_ONLY

		/* USB Device Mode Driver Related Tokens: */
		#define USE_FLASH_DESCRIPTORS
		#define FIXED_CONTROL_ENDPOINT_SIZE      8
		#define FIXED_NUM_CONFIGURATIONS         1

		/* USB Host Mode Driver Related Tokens: */

	#elif (ARCH == ARCH_XMEGA)

		/* Non-USB Related Configuration Tokens: */

		/* USB Class Driver Related Tokens: */

		/* General USB Driver Related Tokens: */
		#define USE_STATIC_OPTIONS               (USB_DEVICE_OPT_FULLSPEED | USB_OPT_RC32MCLKSRC | USB_OPT_BUSEVENT_PRIHIGH)

		/* USB Device Mode Driver Related Tokens: */
		#define USE_FLASH_DESCRIPTORS
		#define FIXED_CONTROL_ENDPOINT_SIZE      8
		#define FIXED_NUM_CONFIGURATIONS         1
		#define MAX_ENDPOINT_INDEX               2

	#else

		#error Unsupported architecture for this LUFA configuration file.

	#endif
#endif

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


#include "midipod.h"
#include "io.h"

#define SWITCH_ON	0x01
#define SWITCH_OFF	0x00
#define DEBOUNCE_TIME	25
#define BASE_NOTE	12

uint8_t status, channel, data1, data2, p_state, breathlevel;

MIDI_EventPacket_t Uart_MIDI_Event;

USB_ClassInfo_MIDI_Device_t Universal_MIDI_Interface =
{
	.Config =
	{
		.StreamingInterfaceNumber = INTERFACE_ID_AudioStream,
		.DataINEndpoint           =
		{
			.Address          = MIDI_STREAM_IN_EPADDR,
			.Size             = MIDI_STREAM_EPSIZE,
			.Banks            = 1,
		},
		.DataOUTEndpoint          =
		{
			.Address          = MIDI_STREAM_OUT_EPADDR,
			.Size             = MIDI_STREAM_EPSIZE,
			.Banks            = 1,
		},
	},
};

int footswitch_is_pressed (void)
{
	if (bit_is_clear(PIND,3)) {
		_delay_ms(DEBOUNCE_TIME);
		if ( bit_is_clear (PIND,3)) {
			while ( bit_is_clear (PIND,3)) {
				MIDI_Device_USBTask(&Universal_MIDI_Interface);
				USB_USBTask();
			}
			return 1;
		}
	}
	return 0;
}

void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	USB_Init();

	/* Toggle switch configuration */
	clearBIT(DDRD,1);	
	setBIT(PORTD,1);
	clearBIT(DDRD,2);	
	setBIT(PORTD,2);

	/* Pedal switch configuration */
	clearBIT(DDRD,3);	
	setBIT(PORTD,3);

	/* LEDs configuration */
	setBIT(DDRC,2);		/* Switch Led */
	setBIT(DDRD,0);		/* USB Led */	

	_delay_ms(200);
	setBIT(PORTD,0);
	_delay_ms(200);
	clearBIT(PORTD,0);
}

void EVENT_USB_Device_Connect(void)
{
	// Turn on USB LED
	setBIT(PORTD,0);
}

void EVENT_USB_Device_Disconnect(void)
{
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;
	ConfigSuccess &= MIDI_Device_ConfigureEndpoints(&Universal_MIDI_Interface);
}

void EVENT_USB_Device_ControlRequest(void)
{
	MIDI_Device_ProcessControlRequest(&Universal_MIDI_Interface);
}

int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();
	p_state  = SWITCH_OFF;

	while (1) {
		if ( footswitch_is_pressed() ) {
			if (p_state == SWITCH_OFF) {
				/* Note ON */
				p_state=SWITCH_ON;
				setBIT(PORTC,2);
				status=0x90;
			}
			else {
				/* Note OFF */
				p_state=SWITCH_OFF;
				clearBIT(PORTC,2);
				status=0x80;
			}

			if ( bit_is_set (PIND,1) && bit_is_clear (PIND,2) ) breathlevel=80;
			if ( bit_is_set (PIND,1) && bit_is_set (PIND,2) ) breathlevel=50;
			if ( bit_is_clear (PIND,1) && bit_is_set (PIND,2) ) breathlevel=30;
			channel=0x01;
			data1= BASE_NOTE;
			data2=100;
			Uart_MIDI_Event.Event = MIDI_EVENT(0, status);

			/* Envoi du note ON/OFF */
			Uart_MIDI_Event.Data1 = status | channel;
			Uart_MIDI_Event.Data2 = data1;
			Uart_MIDI_Event.Data3 = data2;
			MIDI_Device_SendEventPacket(&Universal_MIDI_Interface, &Uart_MIDI_Event);
			MIDI_Device_Flush(&Universal_MIDI_Interface);
		}

		/* Envoi du Breath control */
		if ( status == 0x90 ) {
			Uart_MIDI_Event.Event = MIDI_EVENT(0, 0xB0);
			Uart_MIDI_Event.Data1 = 0xB0 | channel;
			Uart_MIDI_Event.Data2 = 0x02;
			Uart_MIDI_Event.Data3 = breathlevel;
			MIDI_Device_SendEventPacket(&Universal_MIDI_Interface, &Uart_MIDI_Event);
			MIDI_Device_Flush(&Universal_MIDI_Interface);
			_delay_ms(5);
		}

		MIDI_Device_USBTask(&Universal_MIDI_Interface);
		USB_USBTask();
	}
}

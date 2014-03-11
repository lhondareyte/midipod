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
#define BASE_NOTE	0x23

uint8_t status, channel, data1, data2, p_state;

volatile MIDI_EventPacket_t Uart_MIDI_Event;

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
	if (bit_is_clear(PINB,7))
	{
		_delay_ms(DEBOUNCE_TIME);
		if ( bit_is_clear (PINB,7))
		{
			while ( bit_is_clear (PINB,7))
			{
				MIDI_Device_USBTask(&Universal_MIDI_Interface);
				USB_USBTask();
			}
			return 1;
		}
	}
	return 0;
}

int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();
	p_state  = SWITCH_OFF;
	
	for (;;)
	{

		if ( footswitch_is_pressed() )
		{

			if (p_state == SWITCH_OFF) 
			{
				p_state=SWITCH_ON;
				status=0x90;
			}
			else 
			{
				p_state=SWITCH_OFF;
				status=0x80;
			}

			channel=0x01;
			data1=0x23;
			data2=100;
                        Uart_MIDI_Event.Event = MIDI_EVENT(0, status);
                        Uart_MIDI_Event.Data1 = status|channel;
                        Uart_MIDI_Event.Data2 = data1;
                        Uart_MIDI_Event.Data3 = data2;
			MIDI_Device_SendEventPacket(&Universal_MIDI_Interface, &Uart_MIDI_Event);
                	MIDI_Device_Flush(&Universal_MIDI_Interface);
		}
		
		MIDI_Device_USBTask(&Universal_MIDI_Interface);
		USB_USBTask();
	}
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

	/* ADC initialisation */

	/* Pedal switch configuration */
	clearBIT(DDRB,7);;	

	/* Debug LED */
	setBIT(DDRB,6);;	
}


void EVENT_USB_Device_Connect(void)
{
	setBIT(PORTB,6);
	_delay_ms(200);
	clearBIT(PORTB,6);
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


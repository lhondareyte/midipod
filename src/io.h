 /*
  * Copyright (c) 2006-2011 Luc HONDAREYTE
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions
  * are met:
  * 1. Redistributions of source code must retain the above copyright
  *    notice, this list of conditions and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
  * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  *  $Id: io.h,v 1.5 2012/04/15 16:03:32 luc Exp luc $
  */


#ifndef __LOFITECH_AVR_IO_H__
 #define __LOFITECH_AVR_IO_H__

#if defined (__AVR__)
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

/*
 * Pseudo fonctions
 */

#define setBIT(octet,bit)	( octet |= (1<<bit))
#define clearBIT(octet,bit)	( octet &= ~(1<<bit))
#define DisableInterrupt()	clearBIT(SREG,SREG_I)
#define EnableInterrupt()	setBIT(SREG,SREG_I)

#if defined (__AVR_AT90S2313__) 
 #define UART_RECEIV_VECT	UART_RX_vect
 #define uartSendByte(data)	loop_until_bit_is_set(USR, UDRE); UDR=data;
 #define setBAUD(speed)		( UBRRL = (F_CPU /(16L * speed))-1 )
 #define UART_REG               UCR
#endif

#if defined (__AVR_ATtiny2313__)
 #define UART_RECEIV_VECT	USART_RX_vect
 #define uartSendByte(data)	loop_until_bit_is_set(UCSRA, UDRE); UDR=data;
 #define setBAUD(speed)		UBRRH = (unsigned char) ((( F_CPU/( 16L * speed )) - 1) >> 8); UBRRL = (unsigned char) (( F_CPU/( 16L * speed )) - 1)
 #define UART_REG               UCSRB
 #define TCCR0                  TCCR0B
#endif

#if defined (__AVR_ATtiny4313__)
 #define UART_RECEIV_VECT	USART_RX_vect
 #define uartSendByte(data)	loop_until_bit_is_set(UCSRA, UDRE); UDR=data;
 #define setBAUD(speed)		UBRRH = (unsigned char) ((( F_CPU/( 16L * speed )) - 1) >> 8); UBRRL = (unsigned char) (( F_CPU/( 16L * speed )) - 1)
 #define UART_REG               UCSRB
 #define TCCR0                  TCCR0B
#endif

#if defined (__AVR_ATmega48__)
 #define UART_RECEIV_VECT	USART_RXC_vect
 #define uartSendByte(data)	loop_until_bit_is_set(UCSR0A, UDRE0); UDR0=data;
 #define setBAUD(speed)		UBRR0H = (uint8_t) ( (( F_CPU / ( 16L * speed ) ) - 1) >> 8 ); UBRR0L = (uint8_t) (( F_CPU / ( 16L * speed ) ) - 1)
 #define UART_REG		UCSR0C
 #define TCCR0                  TCCR0A
 #define TIMSK                  TIMSK0
#endif

#if defined (__AVR_ATmega8__)
 #define UART_RECEIV_VECT	USART_RXC_vect
 #define uartSendByte(data)	loop_until_bit_is_set(UCSRA, UDRE); UDR=data;
 #define setBAUD(speed)		UBRRH = (uint8_t) ( (( F_CPU / ( 16L * speed ) ) - 1) >> 8 ); UBRRL = (uint8_t) (( F_CPU / ( 16L * speed ) ) - 1)
 #define UART_REG		UCSRC
#endif 

#if defined (__AVR_ATmega8U2__)
 #define UART_IO		UDR1
 #define UART_RECEIV_VECT	USART1_RX_vect
 #define uartSendByte(data)	loop_until_bit_is_set(UCSR1A, UDRE1); UDR1=data;
 #define setBAUD(speed)		UBRR1H = (uint8_t) ( (( F_CPU / ( 16L * speed ) ) - 1) >> 8 ); UBRR1L = (uint8_t) (( F_CPU / ( 16L * speed ) ) - 1)
 #define UART_REG		UCSR1B
#endif 

#if defined (__AVR_ATmega8535__)
#warning "AT mega8535 not implemented in io.h"
#endif


#endif // __AVR__
#endif // __LOFITECH_AVR_IO_H__

/*
 * usart.c
 *
 *  Created on: 15 kwi 2018
 *      Author: alf64
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

void UsartInit(uint16_t ubrrvalue)
{
    UBRRH |= (uint8_t)(ubrrvalue>>8);
    UBRRL |= (uint8_t)(ubrrvalue); // ustawienie predkosci transmisji

    UCSRB |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);        // odpalenie nadajnika, odbiornika + przerwanie o nadaniu + przerwanie o odbiorze
    UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); // 8N1: 1 bit stopu (bit SBS = 0), UCSZ1 i 0 ustawiaja 8bitow danych, URSEL wymagany by zapisac UCSR0C
    UCSRA |= (1<<U2X); //double speed of uart
}


void UsartTransmit(uint8_t data)
{
    while(!(UCSRA&(1<<UDRE))); //oczekiwanie az UDRE bedzie '1' - wtedy gotowosc do wysylania
    UDR = data;
}


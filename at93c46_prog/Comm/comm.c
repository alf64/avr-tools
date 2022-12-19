/*
 * comm.c
 *
 *  Created on: 15 kwi 2018
 *      Author: alf64
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "usart.h"
#include "comm.h"


char rcvbuff[COMM_RECEIVE_BUFFER_SIZE];
char* rcvmsg_start = rcvbuff;
char* rcvmsg_end = rcvbuff;

comm_status_t CommInit(
        uint32_t baud_rate,
        uint32_t f_cpu,
        unsigned const char* init_msg,
        uint8_t init_msg_size)
{
    comm_status_t status = COMM_SUCCESS;

    switch(baud_rate)
    {
        case 19200:
        case 38400:
        case 57600:
        {
            uint16_t ubrr = 0;
            uint32_t rest = (f_cpu % (8*baud_rate));
            if(rest > 0)
                return COMM_FAILED; // error rate > 0 not supported
            else
            {
                ubrr = (uint16_t)((f_cpu / (8*baud_rate)) - 1);
                // ubrr = 71 for 19200bps and f_cpu: 11.0592 Mhz
                // ubrr = 35 for 38400bps and f_cpu: 11.0592 Mhz
                // ubrr = 23 for 38400bps and f_cpu: 11.0592 Mhz
                UsartInit(ubrr);
            }
            break;
        }
        default:
        {
            return COMM_FAILED; // unsupported baud rate with given f_cpu
        }
    }

    if(init_msg == NULL)
        CommSendMsgFromFlash(defaultmsg, (sizeof(defaultmsg)-1), 1);
    else
        CommSendMsg(init_msg, init_msg_size, 1);

    return status;
}


comm_status_t CommSendMsg(
        unsigned const char* msg,
        uint8_t msg_size,
        uint8_t nl)
{
    comm_status_t status = COMM_SUCCESS;

    if((msg == NULL) || (msg_size == 0))
        return COMM_FAILED;

    for(uint8_t i = 0; i < msg_size; i++)
    {
        UsartTransmit((uint8_t)msg[i]);
    }

    if(nl)
    {
        // transmit new line character
        UsartTransmit(0x0a);
    }

    return status;
}

comm_status_t CommSendMsgFromFlash(
        const char* PROGMEM msg,
        uint8_t msg_size,
        uint8_t nl)
{
    comm_status_t status = COMM_SUCCESS;

    if((msg == NULL) || (msg_size == 0))
        return COMM_FAILED;

    for(uint8_t i = 0; i < msg_size; i++)
    {
        UsartTransmit((uint8_t)pgm_read_byte(&msg[i]));
    }

    if(nl)
    {
        // transmit new line character
        UsartTransmit(0x0a);
    }

    return status;
}

comm_status_t CommSendMsgsArrFromFlash(
        const char* PROGMEM msg_arr,
        uint8_t msgs_count,
        uint8_t msgs_max_chrs)
{
    comm_status_t status = COMM_SUCCESS;

    if((msg_arr == NULL) || (msgs_count == 0))
        return COMM_FAILED;

    const char* PROGMEM cursor = msg_arr;
    uint8_t msgs_read = 0;
    uint8_t msg_chrs_cnt = 0;

    while(msgs_read < msgs_count)
    {
        if(msg_chrs_cnt < msgs_max_chrs)
        {
            uint8_t bt_read = pgm_read_byte(cursor);
            if(bt_read != '\0')
            {
                UsartTransmit(bt_read);
                msg_chrs_cnt++;
            }
            else
            {
                if(pgm_read_byte((cursor+1)) != '\0')
                {
                    // end of message detected
                    UsartTransmit(0x0a); // transmit new line character ('\n')
                    msg_chrs_cnt = 0;
                    msgs_read++;
                }
                else
                {
                    msg_chrs_cnt++;
                }
            }
        }
        else
        {
            return COMM_FAILED; // chrs limit per msg exceeded
        }

        cursor++;
    }

    return status;
}

comm_status_t CommSendBytes(
        const uint8_t* data_arr,
        uint32_t data_size)
{
    if((data_arr == NULL) || data_size == 0)
        return COMM_FAILED;

    const uint8_t* dt = data_arr;
    for(uint32_t i = 0; i < data_size; i++)
    {
        UsartTransmit(*dt);
        dt++;
    }

    return COMM_SUCCESS;
}

comm_status_t CommSendByteAsHexAscii(uint8_t data)
{
    comm_status_t status = COMM_SUCCESS;
    const uint8_t ascii_0_offset = 0x30;
    const uint8_t ascii_A_offset = 0x41;

    char hex[2]; hex[0] = 'B'; hex[1] = 'E';

    // conversion form byte to hex ascii
    // TIP: rest can vary form 0 to 15
    uint8_t result = data;
    uint8_t rest = data;
    for(uint8_t i = 0; i < 2; i++)
    {
        rest = result % 16;
        result = result / 16;

        if(rest > 9)
        {
            rest = rest-10;
            hex[i] = ascii_A_offset+rest;
        }
        else
            hex[i] = ascii_0_offset+rest;

        if((result == 0) && (i == 0)) // data < 16
        {
            hex[1] = ascii_0_offset;
            break;
        }
    }

    // transmit chars from most significant one to least significant one
    for(uint8_t i = sizeof(hex); i > 0 ; i--)
    {
        UsartTransmit(hex[(i-1)]);
    }

    return status;
}

void CommCleanMsgBuffer(void)
{
    rcvmsg_end = rcvmsg_start;
    for(uint8_t i = 0; i < COMM_RECEIVE_BUFFER_SIZE; i++)
    {
        rcvbuff[i] = '\0';
    }
}

comm_status_t CommGetMsg(
        uint8_t msg_size,
        unsigned char* dst,
        uint8_t dst_size)
{
    comm_status_t status = COMM_SUCCESS;

    if(rcvmsg_end == NULL || rcvmsg_end == NULL || dst == NULL)
        return COMM_FAILED;

    if(msg_size > COMM_RECEIVE_BUFFER_SIZE)
        return COMM_FAILED;

    if(dst_size < msg_size)
        return COMM_FAILED;

    uint8_t rcvmsg_len = rcvmsg_end - rcvmsg_start;

    if(rcvmsg_len >= msg_size)
    {
        // it's our message... Copy it!
        for(uint8_t i = 0; i < msg_size; i++)
        {
            *dst++ = *rcvmsg_start++;
        }

        // restart rcv buffer. If the buffer has more bytes: we don't care and fry them
        rcvmsg_start = rcvbuff;
        rcvmsg_end = rcvbuff;
    }
    else if(rcvmsg_len < msg_size)
        return COMM_RECEIVE_MSG_NOT_READY;
    else if(rcvmsg_len == 0)
        return COMM_RECEIVE_BUFF_EMPTY;
    else
    {
        // dunno what to do here ...
        return COMM_FAILED;
    }

    return status;
}


// isr upon a receive on UART...
ISR(USART_RXC_vect)
{
    if(rcvmsg_start != NULL && rcvmsg_end != NULL
            && rcvmsg_end < rcvbuff + sizeof(rcvbuff))
    {
        *rcvmsg_end++ = UDR;
    }
    else
    {
        // rcvbuff may be full, do nothing. Someone needs to read rcvbuff.
    }
}

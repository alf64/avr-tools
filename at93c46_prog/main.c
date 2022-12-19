/*
 * main.c
 *
 *  Created on: 11 cze 2019
 *      Author: bmudlaff
 */


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Comm/comm.h"
#include "usr_msg.h"
#include "at93c46.h"

//!< Maximum user input (in bytes) from comm
#define MAX_USR_INPUT 1
unsigned char usr_input[MAX_USR_INPUT];

//!< Organization of the AT93C46.
#define AT93C46_ORG AT93C46_ORGANIZATION_128x8

uint8_t addr = 0;
#if AT93C46_ORG == AT93C46_ORGANIZATION_128x8
uint8_t data = 0;
uint8_t data_width = 1;
uint8_t addr_limit = 128;
#elif AT93C46_ORG == AT93C46_ORGANIZATION_64x16
uint16_t data = 0;
uint8_t data_width = 2;
uint8_t addr_limit = 64;
#else
#error "AT93C46_ORG value invalid or not defined."
#endif

uint8_t data_all[AT93C46_CAPACITY];

int main(void)
{
    // turn ON interrupts
    sei();

    // init communication (via UART) mechanism
    comm_status_t c_stat = CommInit(
            57600,
            F_CPU,
            NULL,
            0);
    if(c_stat != COMM_SUCCESS)
        while(1); // stuck here forever

    at93c46_init();

    for(;;)
    {
        UsrMsgDispMenu();

        // get user input
        CommCleanMsgBuffer();
        do
        {
            c_stat = CommGetMsg(1, usr_input, sizeof(usr_input));
        } while(c_stat != COMM_SUCCESS);
        CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

        switch(usr_input[0])
        {
            case USR_MSG_READ_CHOICE:
            {
                usr_msg_status_t msg_stat = UsrMsgAskForAddr(&addr);
                if(msg_stat != USR_MSG_SUCCESS)
                {
                    UsrMsgDispCriticalErr();
                    while(1){}; // halt forever
                }

                at93c46_status_t at93_stat =
                        at93c46_read(
                                addr,
                                AT93C46_ORG,
                                &data);
                if(at93_stat == AT93C46_FAILURE)
                {
                    UsrMsgDispCriticalErr();
                    while(1){}; // halt forever
                }
                else if(at93_stat == AT93C46_READ_ERR)
                {
                    UsrMsgDispReadErr();
                    break;
                }

                msg_stat = UsrMsgDispData(AT93C46_ORG, &data);
                if(msg_stat != USR_MSG_SUCCESS)
                {
                    UsrMsgDispCriticalErr();
                    while(1){}; // halt forever
                }

                UsrMsgDispJobDone();

                break;
            }
            case USR_MSG_WRITE_CHOICE:
            {
                usr_msg_status_t msg_stat = UsrMsgAskForAddr(&addr);
                if(msg_stat != USR_MSG_SUCCESS)
                {
                    UsrMsgDispCriticalErr();
                    while(1){}; // halt forever
                }

                msg_stat = UsrMsgAskForData(AT93C46_ORG, &data);
                if(msg_stat != USR_MSG_SUCCESS)
                {
                    UsrMsgDispCriticalErr();
                    while(1){}; // halt forever
                }

                uint8_t proceed = 0;
                msg_stat = UsrMsgAskForProceed(&proceed);
                if(msg_stat != USR_MSG_SUCCESS)
                {
                    UsrMsgDispCriticalErr();
                    while(1){}; // halt forever
                }

                if(proceed)
                {
                    at93c46_ewen(AT93C46_ORG);

                    at93c46_status_t at93_stat =
                            at93c46_write(
                                    addr,
                                    AT93C46_ORG,
                                    &data);
                    if(at93_stat != AT93C46_SUCCESS)
                    {
                        UsrMsgDispCriticalErr();
                        while(1){}; // halt forever
                    }

                    at93c46_ewds(AT93C46_ORG);

                    UsrMsgDispJobDone();
                }

                break;
            }
            case USR_MSG_READALL_CHOICE:
            {
                UsrMsgDispReadAll();

                uint8_t proceed = 0;
                usr_msg_status_t msg_stat = UsrMsgAskForProceed(&proceed);
                if(msg_stat != USR_MSG_SUCCESS)
                {
                    UsrMsgDispCriticalErr();
                    while(1){}; // halt forever
                }

                if(proceed)
                {
                    for(uint8_t i = 0; i < addr_limit; i++)
                    {
                        at93c46_status_t at93_stat =
                                at93c46_read(
                                        i,
                                        AT93C46_ORG,
                                        &data);
                        if(at93_stat == AT93C46_FAILURE)
                        {
                            UsrMsgDispCriticalErr();
                            while(1){}; // halt forever
                        }
                        else if(at93_stat == AT93C46_READ_ERR)
                        {
                            UsrMsgDispReadErr();
                            break;
                        }

                        CommSendBytes(&data, data_width);
                    }

                    while(1){} // halt forever
                }

                break;
            }
            case USR_MSG_WRITEALL_CHOICE:
            {
                UsrMsgDispWriteAll();

                uint8_t proceed = 0;
                usr_msg_status_t msg_stat = UsrMsgAskForProceed(&proceed);
                if(msg_stat != USR_MSG_SUCCESS)
                {
                    UsrMsgDispCriticalErr();
                    while(1){}; // halt forever
                }

                if(proceed)
                {
                    msg_stat = UsrMsgAskForAllData(
                            AT93C46_ORG,
                            data_all,
                            sizeof(data_all));
                    if(msg_stat != USR_MSG_SUCCESS)
                    {
                        UsrMsgDispCriticalErr();
                        while(1){}; // halt forever
                    }

                    at93c46_ewen(AT93C46_ORG);

                    for(uint8_t i = 0; i < AT93C46_CAPACITY; i++)
                    {
                        at93c46_status_t at93_stat =
                                at93c46_write(
                                        i,
                                        AT93C46_ORG,
                                        &data_all[i]);
                        if(at93_stat != AT93C46_SUCCESS)
                        {
                            UsrMsgDispCriticalErr();
                            while(1){}; // halt forever
                        }
                    }

                    at93c46_ewds(AT93C46_ORG);

                    UsrMsgDispJobDone();
                }

                break;
            }
            default:
            {
                UsrMsgDispUnsupportedSel();
                break;
            }
        }
    }

    for(;;);

    return 0;
}

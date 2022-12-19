/*
 * usr_msg.c
 *
 *  Created on: 12 cze 2019
 *      Author: bmudlaff
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Comm/comm.h"
#include "usr_msg.h"

const char usr_msg_main_menu[] PROGMEM = {
        "Welcome to at93c46_prog!\n"
        "Choose one of the following:\n"
        "0x1: Read byte.\n"
        "0x2: Write byte.\n"
        "0x3: Read all 128 bytes.\n"
        "0x4: Write all 128 bytes.\n"
        "Select [1 - 4] (input raw byte): "
};

const char usr_msg_proceed_prompt[] PROGMEM = {
        "Proceed ? (YES: 0x79, NO: anything other): "
};

const char usr_msg_input_received[16] PROGMEM = {
        "Input received."
};

const char usr_msg_unsupported_sel[] PROGMEM = {
        "Unsupported selection."
};

const char usr_msg_addr_prompt[] PROGMEM = {
        "Provide address (input raw byte): "
};

const char usr_msg_data_u8_raw_prompt[] PROGMEM = {
        "Provide u8 data (raw byte): "
};

const char usr_msg_data_u16_raw_prompt[] PROGMEM = {
        "Provide u16 data (raw bytes LSB, MSB): "
};

const char usr_msg_critical_err[] PROGMEM = {
        "Critical error, system halted here."
};

const char usr_msg_read_err[] PROGMEM = {
        "Read error."
};

const char usr_msg_job_done[] PROGMEM = {
        "Job done. "
};

const char usr_msg_disp_data_u8[] PROGMEM = {
        "Displaying u8 data (single raw byte),\n"
        "preceded and followed by new-line char: "
};

const char usr_msg_disp_data_u16[] PROGMEM = {
        "Displaying u16 data (raw bytes LSB, MSB),\n"
        "preceded and followed by new-line char: "
};

const char usr_msg_readall_info[] PROGMEM = {
        "About to read all memory."
        "Data will be displayed as raw bytes."
        "Upon all data read, program will halt forever."
};

const char usr_msg_writeall_info[] PROGMEM = {
        "About to write all memory."
};

const char usr_msg_data_all_prompt_u8[] PROGMEM = {
        "Organization is 128 x 8."
        "Please provide 128 bytes."
        "Upon all data write, program will display job done message."
};

const char usr_msg_data_all_prompt_u16[] PROGMEM = {
        "Organization is 64 x 16."
        "Please provide 128 bytes. Use u16 order: LSB, MSB."
        "Upon all data write, program will display job done message."
};


void UsrMsgDispMenu(void)
{
    CommSendMsgFromFlash(
            usr_msg_main_menu,
            (sizeof(usr_msg_main_menu)-1),
            1);
}

usr_msg_status_t UsrMsgAskForProceed(uint8_t* proceed)
{
    if(proceed == NULL)
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_proceed_prompt,
            (sizeof(usr_msg_proceed_prompt)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(1, proceed, 1) != COMM_SUCCESS); // using proceed to obtain input

    if(*proceed == 0x79)
    {
        *proceed = 1;
    }
    else
    {
        *proceed = 0;
    }

    return USR_MSG_SUCCESS;
}

usr_msg_status_t UsrMsgAskForAddr(uint8_t* addr)
{
    if(addr == NULL)
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_addr_prompt,
            (sizeof(usr_msg_addr_prompt)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(1, addr, 1) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    return USR_MSG_SUCCESS;
}

usr_msg_status_t UsrMsgAskForData(
        uint8_t org,
        void* data)
{
    if(data == NULL)
        return USR_MSG_FAILED;

    if(org)
    { // 64 x 16
        CommSendMsgFromFlash(
                usr_msg_data_u16_raw_prompt,
                (sizeof(usr_msg_data_u16_raw_prompt)-1),
                1);
        CommCleanMsgBuffer();
        while(CommGetMsg(2, data, 2) != COMM_SUCCESS);
    }
    else
    { // 128 x 8
        CommSendMsgFromFlash(
                usr_msg_data_u8_raw_prompt,
                (sizeof(usr_msg_data_u8_raw_prompt)-1),
                1);
        CommCleanMsgBuffer();
        while(CommGetMsg(1, data, 1) != COMM_SUCCESS);
    }

    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    return USR_MSG_SUCCESS;
}

void UsrMsgDispCriticalErr(void)
{
    CommSendMsgFromFlash(
            usr_msg_critical_err,
            (sizeof(usr_msg_critical_err)-1),
            1);
}

void UsrMsgDispReadErr(void)
{
    CommSendMsgFromFlash(
            usr_msg_read_err,
            (sizeof(usr_msg_read_err)-1),
            1);
}

void UsrMsgDispJobDone(void)
{
    CommSendMsgFromFlash(
            usr_msg_job_done,
            (sizeof(usr_msg_job_done)-1),
            1);
}

usr_msg_status_t UsrMsgDispData(
        uint8_t org,
        void* data)
{
    if(data == NULL)
        return USR_MSG_FAILED;

    if(org)
    { // 64 x 16
        CommSendMsgFromFlash(
                usr_msg_disp_data_u16,
                (sizeof(usr_msg_disp_data_u16)-1),
                1);

        CommSendMsg(data, 2, 1); // send 2 bytes since data is uint16_t (x16)
    }
    else
    { // 128 x 8
        CommSendMsgFromFlash(
                usr_msg_disp_data_u8,
                (sizeof(usr_msg_disp_data_u8)-1),
                1);

        CommSendMsg(data, 1, 1); // send 1 byte since data is uint8_t (x8)
    }

    return USR_MSG_SUCCESS;
}

void UsrMsgDispReadAll(void)
{
    CommSendMsgFromFlash(
            usr_msg_readall_info,
            (sizeof(usr_msg_readall_info)-1),
            1);
}

void UsrMsgDispWriteAll(void)
{
    CommSendMsgFromFlash(
            usr_msg_writeall_info,
            (sizeof(usr_msg_writeall_info)-1),
            1);
}

usr_msg_status_t UsrMsgAskForAllData(
        uint8_t org,
        void* data,
        uint32_t data_size)
{
    if(data == NULL)
        return USR_MSG_FAILED;

    if(org)
    { // 64 x 16
        CommSendMsgFromFlash(
                usr_msg_data_all_prompt_u16,
                (sizeof(usr_msg_data_all_prompt_u16)-1),
                1);
    }
    else
    { // 128 x 8
        CommSendMsgFromFlash(
                usr_msg_data_all_prompt_u8,
                (sizeof(usr_msg_data_all_prompt_u8)-1),
                1);
    }
    CommCleanMsgBuffer();
    while(CommGetMsg(128, data, data_size) != COMM_SUCCESS);

    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    return USR_MSG_SUCCESS;
}

void UsrMsgDispUnsupportedSel(void)
{
    CommSendMsgFromFlash(
            usr_msg_unsupported_sel,
            (sizeof(usr_msg_unsupported_sel)-1),
            1);
}

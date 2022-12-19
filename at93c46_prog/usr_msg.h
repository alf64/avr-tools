/*
 * usr_msg.h
 *
 *  Created on: 12 cze 2019
 *      Author: bmudlaff
 */

#ifndef USR_MSG_H_
#define USR_MSG_H_

#include <avr/pgmspace.h>


typedef enum
{
    USR_MSG_SUCCESS = 0,
    USR_MSG_FAILED = 1,
    USR_MSG_INVALID_INPUT = 2
}usr_msg_status_t;

#define USR_MSG_READ_CHOICE 1
#define USR_MSG_WRITE_CHOICE 2
#define USR_MSG_READALL_CHOICE 3
#define USR_MSG_WRITEALL_CHOICE 4

extern const char usr_msg_main_menu[] PROGMEM;
extern const char usr_msg_proceed_prompt[] PROGMEM;
extern const char usr_msg_input_received[16] PROGMEM;
extern const char usr_msg_unsupported_sel[] PROGMEM;
extern const char usr_msg_addr_prompt[] PROGMEM;
extern const char usr_msg_data_raw_prompt[] PROGMEM;
extern const char usr_msg_critical_err[] PROGMEM;
extern const char usr_msg_read_err[] PROGMEM;
extern const char usr_msg_job_done[] PROGMEM;
extern const char usr_msg_disp_data_u8[] PROGMEM;
extern const char usr_msg_disp_data_u16[] PROGMEM;
extern const char usr_msg_readall_info[] PROGMEM;
extern const char usr_msg_writeall_info[] PROGMEM;
extern const char usr_msg_data_all_prompt_u8[] PROGMEM;
extern const char usr_msg_data_all_prompt_u16[] PROGMEM;

/*
 * @brief Displays main menu.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 */
void UsrMsgDispMenu(void);

/*
 * @brief Interactively (using comm) asks user if he wants to proceed further or no.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param proceed A pointer where the result (obtained user decision for proceeding) shall be placed.
 * The result (proceed) may have the following values:
 * 0 - means FALSE
 * 1 - means TRUE
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the proceed decision from user.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 */
usr_msg_status_t UsrMsgAskForProceed(uint8_t* proceed);

/*
 * @brief Interactively (using comm) asks user for an address.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param addr A pointer where the result (obtained address) shall be placed.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the address from user.
 * @retval USR_MSG_FAILED Means the critical error occured.
 */
usr_msg_status_t UsrMsgAskForAddr(uint8_t* addr);

/*
 * @brief Interactively (using comm) asks user for data (u8 or u16, depending on org param).
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param org Organization of the memory that shall be used.
 * 0 - 128 x 8 organization
 * 1 - 64 x 16 organization
 * @param data A pointer where the obtained data shall be placed.
 * The actual width of the data relies on org param.
 * If org is set to 0, then the data will be written as of type uint8_t.
 * If org is set to 1, then the data will be written as of type uint16_t.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the data from user.
 * @retval USR_MSG_FAILED Means the critical error occured.
 */
usr_msg_status_t UsrMsgAskForData(
        uint8_t org,
        void* data);

/*
 * @brief Displays critical error message.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 */
void UsrMsgDispCriticalErr(void);

/*
 * @brief Displays read error message.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 */
void UsrMsgDispReadErr(void);

/*
 * @brief Displays job done message.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 */
void UsrMsgDispJobDone(void);

/*
 * @brief Displays job done message.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param org Organization of the memory that shall be used.
 * 0 - 128 x 8 organization
 * 1 - 64 x 16 organization
 * @param data A pointer where the data to be displayed resides.
 * The actual width of the data relies on org param.
 * If org is set to 0, then the data will be displayed as of type uint8_t.
 * If org is set to 1, then the data will be displayed as of type uint16_t.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to display the data to user.
 * @retval USR_MSG_FAILED Means the critical error occured.
 */
usr_msg_status_t UsrMsgDispData(
        uint8_t org,
        void* data);

/*
 * @brief Displays read all message.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 */
void UsrMsgDispReadAll(void);

/*
 * @brief Displays write all message.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 */
void UsrMsgDispWriteAll(void);

/*
 * @brief Interactively (using comm) asks user for all the data (128 bytes).
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param org Organization of the memory that shall be used.
 * 0 - 128 x 8 organization
 * 1 - 64 x 16 organization
 * @param data A pointer where the obtained data shall be placed.
 * This needs to be of size: at least 128 bytes.
 * @param data_size A size of the given data (at least 128 bytes).
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the data from user.
 * @retval USR_MSG_FAILED Means the critical error occured.
 */
usr_msg_status_t UsrMsgAskForAllData(
        uint8_t org,
        void* data,
        uint32_t data_size);

/*
 * @brief Displays unsupported selection message.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 */
void UsrMsgDispUnsupportedSel(void);

#endif /* USR_MSG_H_ */

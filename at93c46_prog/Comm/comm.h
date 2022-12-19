/*
 * comm.h
 *
 *  Created on: 15 kwi 2018
 *      Author: alf64
 */

#ifndef COMM_COMM_H_
#define COMM_COMM_H_

/*
 * Communicator is for communicating with the other
 * device (PC) via UART interface (as COM port).
 */


/*
 * This defines a maximum receive buffer, which stands
 * for maximum message (in bytes) that can be obtained (received)
 * via UART.
 */
#define COMM_RECEIVE_BUFFER_SIZE 128

typedef enum{
    COMM_SUCCESS,
    COMM_FAILED,
    COMM_RECEIVE_BUFF_EMPTY,
    COMM_RECEIVE_MSG_NOT_READY
}comm_status_t;


/*
 * Default msg for Communication
 */
static const char defaultmsg[] PROGMEM = "Comm initialization... OK\n";

/*
 * Inits the Communicator via UART interface.
 *
 * @details
 * This function initializes the communication mechanism via UART and also
 * sends a message upon successful initialization.
 *
 * @param baud_rate A desired baud rate (bps).
 * @param f_cpu A clock frequency of the CPU.
 * @param init_msg A pointer to a message which will be send right after successful init.
 * If NULL is passed as init_msg, function will send a defaultmsg[].
 * @param init_msg_size A size (in bytes) of the init message.
 *
 * @returns comm_status_t.
 * @retval COMM_SUCCESS Means the function succeeded to init the communicator.
 * @retval COMM_FAILED Means function failed. Probably unsupported arguments were given.
 */
comm_status_t CommInit(
        uint32_t baud_rate,
        uint32_t f_cpu,
        unsigned const char* init_msg,
        uint8_t init_msg_size);

/*
 * Sends message via UART interface.
 *
 * @param msg A pointer to a message.
 * @param msg_size A size (in bytes) of the message.
 * @param nl A new-line flag.
 * If nl > 0 then new-line character will be appended at the end of the message.
 * If nl == 0 then there won't be new-line character appended.
 *
 * @attention
 * This function will only work properly if the CommInit() was performed before.
 * This function appends new-line character at the end of the message.
 *
 * @returns comm_status_t.
 * @retval COMM_SUCCESS Means the function succeeded to init the communicator.
 * @retval COMM_FAILED Means function failed. Probably unsupported arguments were given.
 */
comm_status_t CommSendMsg(
        unsigned const char* msg,
        uint8_t msg_size,
        uint8_t nl);

/*
 * Same as CommSendMsg, but this one sends message that
 * is stored in program memory.
 */
comm_status_t CommSendMsgFromFlash(
        const char* PROGMEM msg,
        uint8_t msg_size,
        uint8_t nl);

/*
 * Sends array of char messages located in program memory, via UART interface.
 *
 * @brief
 * Function expects a pointer to 2-D array of const char type, located in progmem.
 * Function assumes that each message from the array is ended with \0.
 *
 * @param msg_arr A pointer to 2-D array filled with messages, located in progmem.
 * @param msg_count A number of messages in the given array. Max is: 256.
 * @param msgs_max_chrs Maximum characters allowed per each message.
 *
 * @attention
 * This function will only work properly if the CommInit() was performed before.
 * Maximum size of each message in the array is: 256 chars (including '\0')
 *
 * @returns comm_status_t.
 * @retval COMM_SUCCESS Means the function succeeded to init the communicator.
 * @retval COMM_FAILED Means function failed. Probably unsupported arguments were given.
 */
comm_status_t CommSendMsgsArrFromFlash(
        const char* PROGMEM msg_arr,
        uint8_t msgs_count,
        uint8_t msgs_max_chrs);

/*
 * @brief Sends given data as bytes via uart interface.
 *
 * @param data_arr Pointer to data.
 * @param data_size A size (in bytes) of the given data_arr.
 *
 * @returns comm_status_t
 * @retval COMM_FAILED Means the function failed (probably bad arguments were given).
 * @retval COMM_SUCCESS Means the function succeeded.
 *
 */
comm_status_t CommSendBytes(
        const uint8_t* data_arr,
        uint32_t data_size);

/*
 * @brief
 * Sends a byte of data as hex (readable as ASCII in terminal).
 *
 * @details
 * For example, if you put 16 as data, this will send as "0x10" (without '0x' extension of course).
 *
 * @param data A byte of data to be send.
 * @returns comm_status_t.
 * @retval COMM_SUCCESS Means the function succeeded to send the data.
 * @retval COMM_FAILED Means the function failed.
 */
comm_status_t CommSendByteAsHexAscii(uint8_t data);

/*
 * @brief Marks receive msg buffer as empty and fills it with NULLs.
 */
void CommCleanMsgBuffer(void);

/*
 * Gets a message from receive buffer.
 *
 * @param msg_size An (expected) size (in bytes) of the message to read.
 * @param dst A pointer to the buffer where the message shall be copied to.
 * @param dst_size A size (in bytes) of the dst buffer.
 *
 * @attention
 * This function will only work properly if the CommInit() was performed before.
 * The receive buffer will be totally flushed (restarted) upon a successful read
 * by this function (COMM_SUCCESS).
 *
 * @returns comm_status_t.
 * @retval COMM_SUCCESS Means the function succeeded to get a message of a given msg_size.
 * @retval COMM_RECEIVE_MSG_NOT_READY Means that something is in receive buffer,
 * but it is not yet a complete message.
 * @retval COMM_RECEIVE_BUFF_EMPTY Means the receive buffer is still empty.
 * @retval COMM_FAILED Means function failed. Probably unsupported arguments were given.
 */
comm_status_t CommGetMsg(
        uint8_t msg_size,
        unsigned char* dst,
        uint8_t dst_size);

#endif /* COMM_COMM_H_ */

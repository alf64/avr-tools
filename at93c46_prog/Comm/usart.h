/*
 * usart.h
 *
 *  Created on: 15 kwi 2018
 *      Author: alf64
 */

#ifndef USARTDRV_USART_H_
#define USARTDRV_USART_H_


/*
 * @brief Inits USART.
 *
 * @param ubrrvalue A value of the UBRR register.
 * Calculate it yourself, depending on the desired BAUD rate (bps).
 * Use the following formula:
 * UBBR = (F_CPU/8*BAUD) - 1
 */
void UsartInit(uint16_t ubrrvalue);

/*
 * @brief Transmits data over USART.
 *
 * @param data A byte of data to be send.
 *
 * @attention
 * This function will only work properly if you
 * init the USART first (UsartInit).
 */
void UsartTransmit(uint8_t data);

/*
 * @brief ISR upon a receive on UART...
 * Define it yourself in your application.
 */
extern ISR(USART0_RXC_vect);


#endif /* USARTDRV_USART_H_ */

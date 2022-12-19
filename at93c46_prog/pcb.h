/*
 * pcb.h
 *
 *  Created on: 12 cze 2019
 *      Author: bmudlaff
 */

#ifndef PCB_H_
#define PCB_H_

/*
 * @brief This file specifies connections between
 * uc and at93c46
 */

#define UC_PORT1 PORTC
#define UC_DDR1 DDRC
#define UC_PIN1 PINC

#define CS_P PC0
#define SK_P PC1
#define DI_P PC2
#define DO_P PC3
#define ORG_P PC4

//!< us
#define CS_SETUP_TIME 1.0f
#define CS_LOW_TIME 1.0f
#define DI_SETUP_TIME 1.0f
#define DI_HOLD_TIME 1.0f
#define DO_OUTPUT_DELAY 1.0f
//!< ms
#define WRITE_CYCLE_TIME 10

#endif /* PCB_H_ */

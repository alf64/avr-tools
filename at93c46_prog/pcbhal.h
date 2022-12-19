/*
 * pcbhal.h
 *
 *  Created on: 12 cze 2019
 *      Author: bmudlaff
 */

#ifndef PCBHAL_H_
#define PCBHAL_H_

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "pcb.h"

/*
 * @brief Inits pcbhal functionality.
 */
void pcbhal_init(void);

/*
 * @brief Enables at93c46 chip.
 */
void pcbhal_chip_enable();

/*
 * @brief Disables at93c46 chip.
 */
void pcbhal_chip_disable();

/*
 * @brief Runs single clock on SK_P.
 */
void pcbhal_sk_p_single_clock_run();

/*
 * @brief Sets the mmeory organisation of the at93c46.
 * 0 - 128 x 8 organization.
 * 1 - 64 x 16 organization.
 */
void pcbhal_set_org(uint8_t org);

static inline void pcbhal_cs_p_set()
{
    UC_PORT1 |= (1<<CS_P);
}

static inline void pcbhal_cs_p_clear()
{
    UC_PORT1 &= ~(1<<CS_P);
}

static inline void pcbhal_cs_p_toggle()
{
    UC_PORT1 ^= (1<<CS_P);
}

static inline void pcbhal_sk_p_set()
{
    UC_PORT1 |= (1<<SK_P);
}

static inline void pcbhal_sk_p_clear()
{
    UC_PORT1 &= ~(1<<SK_P);
}

static inline void pcbhal_sk_p_toggle()
{
    UC_PORT1 ^= (1<<SK_P);
}

static inline void pcbhal_di_p_set()
{
    UC_PORT1 |= (1<<DI_P);
}

static inline void pcbhal_di_p_clear()
{
    UC_PORT1 &= ~(1<<DI_P);
}

static inline void pcbhal_di_p_toggle()
{
    UC_PORT1 ^= (1<<DI_P);
}

static inline void pcbhal_do_p_set()
{
    UC_PORT1 |= (1<<DO_P);
}

static inline void pcbhal_do_p_clear()
{
    UC_PORT1 &= ~(1<<DO_P);
}

static inline void pcbhal_do_p_toggle()
{
    UC_PORT1 ^= (1<<DO_P);
}

static inline uint8_t pcbhal_do_p_get()
{
    return ((UC_PIN1>>DO_P)&0x01);
}

static inline void pcbhal_org_p_set()
{
    UC_PORT1 |= (1<<ORG_P);
}

static inline void pcbhal_org_p_clear()
{
    UC_PORT1 &= ~(1<<ORG_P);
}

static inline void pcbhal_org_p_toggle()
{
    UC_PORT1 ^= (1<<ORG_P);
}

#endif /* PCBHAL_H_ */

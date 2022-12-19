/*
 * pcbhal.c
 *
 *  Created on: 12 cze 2019
 *      Author: bmudlaff
 */

#include "pcbhal.h"

void pcbhal_init(void)
{
    // set pins as outputs
    UC_DDR1 = ((1<<CS_P) | (1<<SK_P) | (1<<DI_P) | (1<<ORG_P));
    // set pins as inputs
    UC_DDR1 &= ~(1<<DO_P);
}

void pcbhal_chip_enable()
{
    pcbhal_cs_p_set();
}

void pcbhal_chip_disable()
{
    pcbhal_cs_p_clear();
}

void pcbhal_sk_p_single_clock_run()
{
    pcbhal_sk_p_toggle();
    _delay_us(1.0f);
    pcbhal_sk_p_toggle();
    _delay_us(1.0f);
}

void pcbhal_set_org(uint8_t org)
{
    if(org)
    {
        pcbhal_org_p_set();
    }
    else
    {
        pcbhal_org_p_clear();
    }
}

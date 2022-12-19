/*
 * at93c46.c
 *
 *  Created on: 12 cze 2019
 *      Author: bmudlaff
 */

#include "pcbhal.h"
#include "at93c46.h"

void at93c46_init(void)
{
    pcbhal_init();
    pcbhal_chip_disable();
    pcbhal_sk_p_clear();
    pcbhal_di_p_clear();
    pcbhal_do_p_clear();
    pcbhal_set_org(0);
}

at93c46_status_t at93c46_read(
        uint8_t addr,
        uint8_t org,
        void* data)
{
    if(data == NULL)
        return AT93C46_FAILURE;

    pcbhal_set_org(org);

    pcbhal_chip_enable();
    _delay_us(CS_SETUP_TIME);
    pcbhal_di_p_set(); // start bit (1)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();
    pcbhal_di_p_set(); // first op bit (1)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();
    pcbhal_di_p_clear(); // second op bit (0)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();

    uint8_t addr_bits_no = 0;
    uint8_t data_bits_no = 0;
    if(org)
    { // 64 x 16
        addr_bits_no = 6;
        data_bits_no = 16;
    }
    else
    { // 128 x 8
        addr_bits_no = 7;
        data_bits_no = 8;
    }

    // shift addr
    for(uint8_t i = 0; i < addr_bits_no; i++)
    {
        uint8_t bit = (addr>>((addr_bits_no - 1) - i)) & 0x1;
        if(bit)
        {
            pcbhal_di_p_set();
        }
        else
        {
            pcbhal_di_p_clear();
        }

        _delay_us(DI_SETUP_TIME);
        pcbhal_sk_p_single_clock_run();
    }

    /*
     * From app note:
     * "It should be noted that a dummy bit (logic “0”)
     * precedes the 8- or 16-bit data output string"
     */
    _delay_us(DO_OUTPUT_DELAY);
    uint8_t dummy_bit = pcbhal_do_p_get();
    if(dummy_bit > 0)
    {
        // restore previous state
        pcbhal_chip_disable();
        _delay_us(CS_SETUP_TIME);
        pcbhal_di_p_clear();
        return AT93C46_FAILURE; // error, dummy bit shall be 0
    }

    pcbhal_sk_p_single_clock_run();

    // get data
    uint16_t dt = 0;
    for(uint8_t i = 0; i < data_bits_no; i++)
    {
        _delay_us(DO_OUTPUT_DELAY);
        //dt |= ((pcbhal_do_p_get())<<i);
        uint8_t bit = pcbhal_do_p_get();
        if(bit)
        {
            dt |= (1<<((data_bits_no-1)-i));
        }
        else
        {
            dt &= ~(1<<((data_bits_no-1)-i));
        }

        pcbhal_sk_p_single_clock_run();
    }

    // restore previous state
    pcbhal_chip_disable();
    _delay_us(CS_SETUP_TIME);
    pcbhal_di_p_clear();

    // give data to user
    if(org)
    { // 64 x 16
        uint16_t* data_p = (uint16_t*)data;
        *data_p = dt;
    }
    else
    { // 128 x 8
        uint8_t* data_p = (uint8_t*)data;
        *data_p = (uint8_t)dt;
    }

    return AT93C46_SUCCESS;
}

void at93c46_ewen(uint8_t org)
{
    pcbhal_set_org(org);

    pcbhal_chip_enable();
    _delay_us(CS_SETUP_TIME);
    pcbhal_di_p_set(); // start bit (1)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();
    pcbhal_di_p_clear(); // first op bit (0)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();
    pcbhal_di_p_clear(); // second op bit (0)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();

    uint8_t addr_bits_no = 0;
    if(org)
    { // 64 x 16
        addr_bits_no = 6;
    }
    else
    { // 128 x 8
        addr_bits_no = 7;
    }

    // shift addr as all '1', since command is 0b11XXXX or 0b11XXXXX
    pcbhal_di_p_set();
    _delay_us(DI_SETUP_TIME);
    for(uint8_t i = 0; i < addr_bits_no; i++)
    {
        pcbhal_sk_p_single_clock_run();
    }

    // restore previous state
    pcbhal_chip_disable();
    _delay_us(CS_SETUP_TIME);
    pcbhal_di_p_clear();
}

void at93c46_ewds(uint8_t org)
{
    pcbhal_set_org(org);

    pcbhal_chip_enable();
    _delay_us(CS_SETUP_TIME);
    pcbhal_di_p_set(); // start bit (1)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();
    pcbhal_di_p_clear(); // first op bit (0)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();
    pcbhal_di_p_clear(); // second op bit (0)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();

    uint8_t addr_bits_no = 0;
    if(org)
    { // 64 x 16
        addr_bits_no = 6;
    }
    else
    { // 128 x 8
        addr_bits_no = 7;
    }

    // shift addr as all '0', since command is 0b00XXXX or 0b00XXXXX
    pcbhal_di_p_clear();
    _delay_us(DI_SETUP_TIME);
    for(uint8_t i = 0; i < addr_bits_no; i++)
    {
        pcbhal_sk_p_single_clock_run();
    }

    // restore previous state
    pcbhal_chip_disable();
    _delay_us(CS_SETUP_TIME);
    pcbhal_di_p_clear();
}

at93c46_status_t at93c46_write(
        uint8_t addr,
        uint8_t org,
        void* data)
{
    if(data == NULL)
        return AT93C46_FAILURE;

    pcbhal_set_org(org);

    pcbhal_chip_enable();
    _delay_us(CS_SETUP_TIME);
    pcbhal_di_p_set(); // start bit (1)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();
    pcbhal_di_p_clear(); // first op bit (0)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();
    pcbhal_di_p_set(); // second op bit (1)
    _delay_us(DI_SETUP_TIME);
    pcbhal_sk_p_single_clock_run();

    uint8_t addr_bits_no = 0;
    uint8_t data_bits_no = 0;
    if(org)
    { // 64 x 16
        addr_bits_no = 6;
        data_bits_no = 16;
    }
    else
    { // 128 x 8
        addr_bits_no = 7;
        data_bits_no = 8;
    }

    // shift addr
    for(uint8_t i = 0; i < addr_bits_no; i++)
    {
        uint8_t bit = (addr>>((addr_bits_no - 1) - i)) & 0x1;
        if(bit)
        {
            pcbhal_di_p_set();
        }
        else
        {
            pcbhal_di_p_clear();
        }

        _delay_us(DI_SETUP_TIME);
        pcbhal_sk_p_single_clock_run();
    }

    // shift data
    uint16_t dt = 0;
    if(org)
    { // 64 x 16
        dt = *((uint16_t*)(data));
    }
    else
    { // 128 x 8
        dt = *((uint8_t*)(data));
    }
    for(uint8_t i = 0; i < data_bits_no; i++)
    {
        uint8_t bit = (dt>>((data_bits_no - 1) - i)) & 0x1;
        if(bit)
        {
            pcbhal_di_p_set();
        }
        else
        {
            pcbhal_di_p_clear();
        }

        _delay_us(DI_SETUP_TIME);
        pcbhal_sk_p_single_clock_run();
    }

    _delay_ms(WRITE_CYCLE_TIME);

    // restore previous state
    pcbhal_chip_disable();
    _delay_us(CS_SETUP_TIME);
    pcbhal_di_p_clear();

    return AT93C46_SUCCESS;
}

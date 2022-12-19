/*
 * at93c46.h
 *
 *  Created on: 12 cze 2019
 *      Author: bmudlaff
 */

#ifndef AT93C46_H_
#define AT93C46_H_


//!< Possible memory organizations of the at93c46 chip.
#define AT93C46_ORGANIZATION_128x8 0
#define AT93C46_ORGANIZATION_64x16 1

//!< Memory capacity (in bytes) of the at93c46 chip.
#define AT93C46_CAPACITY 128

typedef enum
{
    AT93C46_SUCCESS = 0,
    AT93C46_FAILURE = 1,
    AT93C46_READ_ERR = 2
}at93c46_status_t;

/*
 * @brief Inits at93c46 chip.
 */
void at93c46_init(void);

/*
 * @brief Performs READ operation.
 *
 * @param addr An address from which the data shall be read.
 * @param org Organization of the memory that shall be used.
 * 0 - 128 x 8 organization
 * 1 - 64 x 16 organization
 * @param data A pointer where the read data shall be stored.
 * The actual width of the data relies on org param.
 * If org is set to 0, then the read data will be of type uint8_t.
 * If org is set to 1, then the read data will be of type uint16_t.
 *
 * @attention
 * READ operation works regardless of Erase-Write state (EWEN or EWDS).
 *
 * @returns at93c46_status_t
 * @retval AT93C46_SUCCESS If function succeeded to perform READ operation.
 * @retval AT93C46_FAILURE If function failed to perform READ operation.
 */
at93c46_status_t at93c46_read(
        uint8_t addr,
        uint8_t org,
        void* data);

/*
 * @brief Performs EWEN operation.
 *
 * @details
 * EWEN - Erase-Write-Enable.
 * This operation needs to be performed once, before Erase and Write operations, to
 * turn the at93c46 chip into erase-write enabled state.
 * Such mechanism is for avoiding accidental data corruption.
 *
 * @param org Organization of the memory that shall be used.
 * 0 - 128 x 8 organization
 * 1 - 64 x 16 organization
 */
void at93c46_ewen(uint8_t org);

/*
 * @brief Performs EWDS operation.
 *
 * @details
 * EWDS - Erase-Write-Disable.
 * This operation may be performed once, after Erase and Write operations, to
 * turn the at93c46 chip into erase-write disabled state.
 * Such mechanism is for avoiding accidental data corruption.
 *
 * @attention
 * at93c46 always returns to erase-write disabled state after power loss.
 *
 * @param org Organization of the memory that shall be used.
 * 0 - 128 x 8 organization
 * 1 - 64 x 16 organization
 */
void at93c46_ewds(uint8_t org);

/*
 * @brief Performs WRITE operation.
 *
 * @param addr An address where the data shall be written.
 * @param org Organization of the memory that shall be used.
 * 0 - 128 x 8 organization
 * 1 - 64 x 16 organization
 * @param data A pointer from which the data to be written shall be taken.
 * The actual width of the data relies on org param.
 * If org is set to 0, then the read data will be of type uint8_t.
 * If org is set to 1, then the read data will be of type uint16_t.
 *
 * @attention
 * EWEN operation (at93c46_ewen()) must be performed once, before any WRITE operation.
 * Otherwise this function won't take any effect on at93c46.
 *
 * @returns at93c46_status_t
 * @retval AT93C46_SUCCESS If function succeeded to perform WRITE operation.
 * @retval AT93C46_FAILURE If function failed to perform WRITE operation.
 */
at93c46_status_t at93c46_write(
        uint8_t addr,
        uint8_t org,
        void* data);

#endif /* AT93C46_H_ */

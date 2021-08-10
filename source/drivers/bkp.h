/**
 * @file bkp.h
 * @brief Backup registers
 */

#ifndef BKP_H_
#define BKP_H_

#include <stdint.h>

/**
 * @brief Initialize backup registers
 */
void bkp_init(void);

/**
 * @brief Deinitialize backup registers
 * 
 */
void bkp_deinit(void);

/**
 * @brief Write data to the given backup register
 * 
 * @param u32L_bkp_reg .. backup register BKP_DR1 ... BKP_DR42
 * @param u16L_value .. the value to be written to the backup register
 */
void bkp_write(uint32_t u32L_bkp_reg, uint16_t u16L_value);

/**
 * @brief Read backup register
 * @param u32L_bkp_reg .. backup register BKP_DR1 ... BKP_DR42
 * @return uint16_t the value that was read from the register
 */
uint16_t bkp_read(uint32_t u32L_bkp_reg);

#endif // BKP_H_

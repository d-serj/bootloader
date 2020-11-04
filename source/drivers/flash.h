/**
 * @file flash.h
 * @brief 
 * 
 */

#ifndef DRIVERS_FLASH_H_
#define DRIVERS_FLASH_H_

#include <stdint.h>

/**
 * @brief Write data to the flash
 * @param u32L_start_address .. start address to write the data
 * @param u8PL_input_data .. pointer to the buffer with data
 * @param u16L_num_elements .. size of u8PL_input_data
 * @return status of the operation: bit 0: busy, bit 2: programming error, bit 3: wrong data written,
 *   bit 4: write protect error, bit 5: end of operation.
 */
uint32_t flash_program_data(uint32_t u32L_start_address,
  const uint8_t *u8PL_input_data, uint16_t u16L_num_elements);

/**
 * @brief Read data from the flash
 * @param u32L_start_address .. start address to read data from
 * @param u16L_num_elements  .. number of elements to be read
 *   (less or equal to the size of u8PL_output_data)
 * @param u8PL_output_data .. pointer to the buffer where the data will be written
 */
void flash_read_data(uint32_t u32L_start_address,
  uint16_t u16L_num_elements, uint8_t *u8PL_output_data);

#endif // DRIVERS_FLASH_H_
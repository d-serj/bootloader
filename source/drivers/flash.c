/**
 * @file flash.c
 * @brief 
 * 
 */

#include <stdint.h>

#include <libopencm3/stm32/flash.h>

#define FLASH_OPERATION_ADDRESS ((uint32_t)0x0800f000)
#define FLASH_PAGE_NUM_MAX       127
#define FLASH_PAGE_SIZE          0x800
#define FLASH_WRONG_DATA_WRITTEN 0x80

uint32_t flash_program_data(uint32_t u32L_start_address,
  const uint8_t *u8PL_input_data, uint16_t u16L_num_elements)
{
  uint32_t u32L_current_address = u32L_start_address;
  uint32_t u32L_page_address    = u32L_start_address;
  uint32_t u32L_flash_status    = 0;

  /* check if u32L_start_address is in proper range */
  if ((u32L_start_address - FLASH_BASE) >= (FLASH_PAGE_SIZE * (FLASH_PAGE_NUM_MAX + 1)))
    return 1;

  /* calculate current page address */
  if (u32L_start_address % FLASH_PAGE_SIZE)
    u32L_page_address -= (u32L_start_address % FLASH_PAGE_SIZE);

  flash_unlock();

  /* Erasing page */
  flash_erase_page(u32L_page_address);
  u32L_flash_status = flash_get_status_flags();
  if (u32L_flash_status != FLASH_SR_EOP)
    return u32L_flash_status;

  /* programming flash memory */
  for (uint16_t u16L_i = 0; u16L_i < u16L_num_elements; u16L_i += 4)
  {
    /* programming word data */
    flash_program_word(u32L_current_address + u16L_i, *((uint32_t*)(u8PL_input_data + u16L_i)));
    u32L_flash_status = flash_get_status_flags();
    if (u32L_flash_status != FLASH_SR_EOP)
      return u32L_flash_status;

    /* verify if correct data is programmed */
    if (*((uint32_t*)(u32L_current_address + u16L_i)) != *((uint32_t*)(u8PL_input_data + u16L_i)))
      return FLASH_WRONG_DATA_WRITTEN;
  }

  return 0;
}

void flash_read_data(uint32_t u32L_start_address, uint16_t u16L_num_elements, uint8_t *u8PL_output_data)
{
  uint32_t *u32PL_memory_ptr = (uint32_t*)u32L_start_address;

  for (uint16_t u16L_i = 0; u16L_i < u16L_num_elements / 4; ++u16L_i)
  {
    *(uint32_t*)u8PL_output_data = *(u32PL_memory_ptr + u16L_i);
    u8PL_output_data += 4;
  }
}

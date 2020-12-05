/**
 * @file bkp.c
 * @brief
 */

#include <stdint.h>
#include <stdbool.h>

#include <libopencm3/stm32/f1/pwr.h>

void bkp_write(uint32_t u32L_bkp_reg, uint16_t u16L_value)
{
  pwr_disable_backup_domain_write_protect();
  u32L_bkp_reg = u16L_value;
  pwr_enable_backup_domain_write_protect();
}

/**
 * @file bkp.c
 * @brief
 */

#include <stdint.h>
#include <stdbool.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/f1/bkp.h>

#include <system/assert.h>

void bkp_init(void)
{
  rcc_periph_clock_enable(RCC_PWR);
  rcc_periph_clock_enable(RCC_BKP);
}

void bkp_deinit(void)
{
  rcc_periph_clock_disable(RCC_PWR);
  rcc_periph_clock_disable(RCC_BKP);
}

void bkp_write(uint32_t u32L_bkp_reg, uint16_t u16L_value)
{
  pwr_disable_backup_domain_write_protect();
  u32L_bkp_reg = u16L_value;
  pwr_enable_backup_domain_write_protect();
}

uint16_t bkp_read(uint32_t u32L_bkp_reg)
{
  return *(uint16_t*)u32L_bkp_reg;
}

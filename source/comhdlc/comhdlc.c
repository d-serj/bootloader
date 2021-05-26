/**
 * @file comhdlc.c
 * @brief 
 */

#include <stddef.h>
#include <stdbool.h>

#include <system/assert.h>
#include <drivers/usart_driver.h>
#include <delay.h>

#include "minihdlc.h"
#include "comhdlc.h"

static usart_instance_t objS_uart4;

static void comhdlc_send_byte(uint8_t u8L_byte);
static void comhdlc_callback(const uint8_t *u8PL_data, uint16_t u16L_data_size);

void comhdlc_init(void)
{
  usart_setup(&objS_uart4, eUART4);
  minihdlc_init(comhdlc_send_byte, comhdlc_callback);
}

void comhdlc_deinit(void)
{
  usart_deinit(&objS_uart4);
}

bool comhdlc_get_command(uint16_t *u16PL_command, uint32_t u32L_timeout)
{
  uint32_t u32L_time    = u32L_timeout;
  uint16_t u16L_command = 0;
  bool bL_first         = false;

  while (u32L_time--)
  {
    uint8_t  u8L_byte = 0;
    if (usart_get_byte(&objS_uart4, &u8L_byte, 1))
    {
      if (!bL_first)
      {
        u16L_command = u8L_byte;
        bL_first     = true;
      }
      else
      {
        u16L_command = (uint16_t)u8L_byte << 8;
      }
    }
  }
}

static void comhdlc_send_byte(uint8_t u8L_byte)
{
  usart_send_raw(&objS_uart4, &u8L_byte, 1);
}

static void comhdlc_callback(const uint8_t *u8PL_data, uint16_t u16L_data_size)
{

}

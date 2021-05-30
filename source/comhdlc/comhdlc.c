/**
 * @file comhdlc.c
 * @brief 
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <system/assert.h>
#include <drivers/usart_driver.h>
#include <utilities/toolbox.h>
#include <delay.h>

#include "minihdlc.h"
#include "comhdlc.h"

static usart_instance_t objS_uart4;
static bool bS_data_is_received        = false;
static uint8_t u8PS_comhldc_buff[1024] = { 0 };
static uint16_t u16S_num_of_received   = 0;

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

void comhdlc_send_data(uint8_t *u8PL_buff, uint8_t u8L_data_size)
{
  ASSERT(u8PL_buff != NULL);
  minihdlc_send_frame(u8PL_buff, u8L_data_size);
}

bool comhdlc_get_data(uint8_t *u8PL_buff, uint16_t u16L_buff_size, uint32_t u32L_timeout)
{
  ASSERT(u8PL_buff != NULL);
  ASSERT((u16L_buff_size > 0) && (u32L_timeout > 0));

  uint32_t u32L_time = u32L_timeout;
  bool bL_ret        = false;

  while (u32L_time--)
  {
    uint8_t u8L_byte = 0;
    while (usart_get_byte(&objS_uart4, &u8L_byte, 1))
    {
      minihdlc_char_receiver(u8L_byte);
    }

    if (bS_data_is_received)
    {
      ASSERT(u16S_num_of_received <= u16L_buff_size);
      memcpy(u8PL_buff, u8PS_comhldc_buff, u16S_num_of_received);
      bS_data_is_received = false;
      bL_ret              = true;
    }
  }

  return bL_ret;
}

static void comhdlc_send_byte(uint8_t u8L_byte)
{
  usart_send_raw(&objS_uart4, &u8L_byte, 1);
}

static void comhdlc_callback(const uint8_t *u8PL_data, uint16_t u16L_data_size)
{
  ASSERT(u16L_data_size <= ARRAY_SIZE(u8PS_comhldc_buff));
  
  memcpy(u8PS_comhldc_buff, u8PL_data, u16L_data_size);
  u16S_num_of_received = u16L_data_size;
  bS_data_is_received  = true;
}

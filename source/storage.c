/**
 * @file storage.c
 * @brief Functions to work with image storage. Implementation
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <system/assert.h>
#include <drivers/usart_driver.h>
#include <modem/sim800/sim800.h>
#include <utilities/toolbox.h>
#include <delay.h>

#include "img-header.h"
#include "storage.h"

static usart_instance_t objS_usart2;

#ifndef UTEST
static bool storage_compare_echo(const char *cPL_cmd, uint32_t u32L_cmd_len);
#else
bool storage_compare_echo(const char *cPL_cmd, uint32_t u32L_cmd_len);
#endif


void storage_init(void)
{
  usart_setup(&objS_usart2, eUART2);
  sim800_power_on();
  usart_send_string(&objS_usart2, "ATE\r\n");
  delay(1);
  usart_flush(&objS_usart2);
}

void storage_deinit(void)
{
  usart_deinit(&objS_usart2);
  sim800_power_off();
}

uint32_t storage_get_chunk(const char *cPL_file_name,
                       uint32_t u32L_chunk_addr,
                       uint8_t *u8PL_chunk_buf,
                       uint32_t u32L_chunk_size)
{
  ASSERT(u8PL_chunk_buf != NULL);
  ASSERT(cPL_file_name != NULL);

  const uint8_t u8L_mode = (u32L_chunk_addr == 0) ? 0 : 1;
  char cPL_buff[128]     = { 0 };
  
  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "AT+FSREAD=%s,%d,%lu,%lu\r\n",
    cPL_file_name, u8L_mode, u32L_chunk_size, u32L_chunk_addr);
  usart_send_string(&objS_usart2, cPL_buff);

  if (storage_compare_echo(cPL_buff, strlen(cPL_buff)) == false)
  {
    usart_flush(&objS_usart2);
    return 0;
  }

  uint32_t u32L_idx = 0;
  uint8_t u8L_data  = 0;

  while (u32L_idx <= u32L_chunk_size)
  {
    usart_get_byte(&objS_usart2, &u8L_data, 10);
    u8PL_chunk_buf[u32L_idx] = u8L_data;
    ++u32L_idx;
  }

  return u32L_idx;
}

uint32_t storage_get_file_size(const char *cPL_file_name)
{
  ASSERT(cPL_file_name != NULL);

  const char *cPL_cmd = "AT+FSFLSIZE=";
  char cPL_buff[128]  = { 0 };

  ASSERT(strlen(cPL_file_name) < (ARRAY_SIZE(cPL_buff) - strlen(cPL_cmd) + 1));

  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "%s%s\r\n+FSFLSIZE: ", cPL_cmd, cPL_file_name);
  usart_send_string(&objS_usart2, cPL_buff);

  if (storage_compare_echo(cPL_buff, strlen(cPL_buff)) == false)
  {
    usart_flush(&objS_usart2);
    return 0;
  }

  uint32_t u32L_idx = 0;
  uint8_t u8L_byte  = 0;
  // Get file size
  while (usart_get_byte(&objS_usart2, &u8L_byte, 10))
  {
    cPL_buff[u32L_idx] = (char)u8L_byte;

    ++u32L_idx;

    if (u32L_idx >= ARRAY_SIZE(cPL_buff))
    {
      ASSERT(0);
      usart_flush(&objS_usart2);
      return 0;
    }
  }

  cPL_buff[u32L_idx] = '\0';

  return (uint32_t)atoi(cPL_buff);
}

bool storage_compare_echo(const char *cPL_cmd, uint32_t u32L_cmd_len)
{
  ASSERT(cPL_cmd != NULL);
  ASSERT(cPL_cmd[0] != '\0');
  ASSERT(u32L_cmd_len > 0);

  bool bL_return    = false;
  uint32_t u32L_idx = 0;
  char cL_input     = '\0';

  while (usart_get_byte(&objS_usart2, (uint8_t*)&cL_input, 10))
  {
    if (cPL_cmd[u32L_idx] == cL_input)
    {
      if (u32L_idx == (u32L_cmd_len - 1))
      {
        bL_return = true;
        break;
      }
    }
    else
    {
      bL_return = false;
      break;
    }

    ++u32L_idx;
    if (u32L_idx > u32L_cmd_len)
    {
      ASSERT(0);
      break;
    }
  }

  return bL_return;
}

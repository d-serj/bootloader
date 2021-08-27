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
#include "storage_sim800.h"

typedef struct
{
  storage_t obj_stor;
  usart_instance_t *objP_uart;
  const char *cP_file_name;
} storage_sim800_t;

static storage_sim800_t objS_stor_sim800;


int8_t sim800_open(storage_t *objPL_this, const char *cPL_file_name, uint8_t u8L_mode);
int8_t sim800_close(storage_t *objPL_this);
int8_t sim800_write(storage_t *objPL_this, const uint8_t *u8PL_buff, uint32_t u32L_buff_size, uint32_t *u32PL_bytes_written);
int8_t sim800_read(storage_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_bytes_to_read, uint32_t *u32PL_bytes_read);

storage_t *storage_sim800_init_static(usart_instance_t *objPL_uart)
{
  storage_t *objPL_storage = (storage_t*)&objS_stor_sim800;

  objS_stor_sim800.objP_uart  = objPL_uart;

  objPL_storage->u32_offset              = 0;
  objPL_storage->obj_virtual_table.open  = sim800_open;
  objPL_storage->obj_virtual_table.close = sim800_close;
  objPL_storage->obj_virtual_table.read  = sim800_read;
  objPL_storage->obj_virtual_table.write = sim800_write;

  sim800_power_on();
  // Synchronize baudrate
  usart_send_string(objS_stor_sim800.objP_uart, "AT\r\n");
  delay(10);
  usart_send_string(objS_stor_sim800.objP_uart, "AT\r\n");
  delay(10);
  usart_send_string(objS_stor_sim800.objP_uart, "AT\r\n");
  delay(10);
  usart_flush(objS_stor_sim800.objP_uart);

  usart_send_string(objS_stor_sim800.objP_uart, "ATE1\r\n");
  delay(10);
  usart_flush(objS_stor_sim800.objP_uart);

  return objPL_storage;
}

void storage_sim800_deinit(void)
{
  sim800_power_off();
}

#ifndef UTEST
bool storage_compare_echo(storage_sim800_t *objPL_this, char *cPL_cmd, uint32_t u32L_cmd_len);
#else
bool storage_compare_echo(const char *cPL_cmd, uint32_t u32L_cmd_len);
#endif

int8_t sim800_open(storage_t *objPL_this, const char *cPL_file_name, uint8_t u8L_mode)
{
  storage_sim800_t *objPL_sim800 = (storage_sim800_t*)objPL_this;

  objPL_sim800->cP_file_name = cPL_file_name;
  objPL_this->u8_mode        = u8L_mode;
  objPL_this->u32_offset     = 0;

  if ((u8L_mode == eStorageModeRead) || (u8L_mode == eStorageModeWrite))
  {
    return 0;
  }

  char cPL_str[128] = { 0 };

  snprintf(cPL_str, sizeof(cPL_str), "AT+FSDEL=%s\r\n", objPL_sim800->cP_file_name);
  usart_send_string(objPL_sim800->objP_uart, cPL_str);
  delay(10);
  usart_flush(objPL_sim800->objP_uart);

  snprintf(cPL_str, sizeof(cPL_str), "AT+FSCREATE=%s\r\n", objPL_sim800->cP_file_name);
  usart_send_string(objPL_sim800->objP_uart, cPL_str);
  delay(10);
  usart_flush(objPL_sim800->objP_uart);

  return 0;
}

int8_t sim800_close(storage_t *objPL_this)
{
  objPL_this->u8_mode    = 0;
  objPL_this->u32_offset = 0;

  return 0;
}

int8_t sim800_write(storage_t *objPL_this, const uint8_t *u8PL_buff, uint32_t u32L_buff_size, uint32_t *u32PL_bytes_written)
{
  char cPL_str[64] = { 0 };
  storage_sim800_t *objPL_stor = (storage_sim800_t*)objPL_this;

  if (u8PL_buff == NULL || objPL_stor->cP_file_name == NULL)
  {
    ASSERT(0);
    return -1;
  }

  snprintf(cPL_str, sizeof(cPL_str), "AT+FSWRITE=%s,1,%ld,5\r\n", objPL_stor->cP_file_name, u32L_buff_size);
  usart_send_string(objPL_stor->objP_uart, cPL_str);
  delay(10);
  usart_flush(objPL_stor->objP_uart);

  usart_send_raw(objPL_stor->objP_uart, u8PL_buff, u32L_buff_size);

  // Wait until file will write
  delay(100);
  usart_flush(objPL_stor->objP_uart);

  if (u32PL_bytes_written)
  {
    *u32PL_bytes_written = u32L_buff_size;
  }
	
	return 0;
}

int8_t sim800_read(storage_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_bytes_to_read, uint32_t *u32PL_bytes_read)
{
  storage_sim800_t *objPL_store = (storage_sim800_t*)objPL_this;
  // 0 - Read data at the beginning of the file
  // 1 - Read data at the <offset> of the file
  const uint8_t u8L_mode = (objPL_this->u32_offset == 0) ? 0 : 1;
  char cPL_buff[256]     = { 0 };

  usart_flush(objPL_store->objP_uart);
  
  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "AT+FSREAD=%s,%d,%lu,%lu\r\n",
    objPL_store->cP_file_name, u8L_mode, u32L_bytes_to_read, objPL_this->u32_offset);
  usart_send_string(objPL_store->objP_uart, cPL_buff);
  delay(10);

  uint32_t u32L_idx = 0;
  uint8_t u8L_data  = 0;
  const uint16_t u16L_req_len = strlen(cPL_buff) + 1;

  // Clear echo
  while (u32L_idx < u16L_req_len)
  {
    usart_get_byte(objPL_store->objP_uart, NULL, 10);
    ++u32L_idx;
  }

  u32L_idx = 0;

  while (u32L_idx < u32L_bytes_to_read)
  {
    usart_get_byte(objPL_store->objP_uart, &u8L_data, 10);
    u8PL_buff[u32L_idx] = u8L_data;
    ++u32L_idx;
  }

  usart_flush(objPL_store->objP_uart);

  objPL_this->u32_offset += u32L_idx;
  
  if (u32PL_bytes_read)
  {
    *u32PL_bytes_read = u32L_idx;
  }

  return 0;
}

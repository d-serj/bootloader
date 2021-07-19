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
  uart_num_t e_uart_num;
} storage_sim800_t;

static storage_sim800_t objS_stor_sim800;

/**
 * @brief Get the file chunk from the storage
 * @param u8PL_chunk_buf .. buffer to be filled with data
 * @param u32L_chunk_size .. chunk size to read
 * @return uint32_t number of read bytes
 */

//static uint32_t storage_get_chunk(storage_sim800_t *objPL_this, uint8_t *u8PL_chunk_buf, uint32_t u32L_chunk_size);

int8_t sim800_open(storage_t *objPL_this, const char *cPL_file_name, uint8_t u8L_mode);
int8_t sim800_close(storage_t *objPL_this);
int8_t sim800_write(storage_t *objPL_this, const uint8_t *u8PL_buff, uint32_t u32L_buff_size, uint32_t *u32PL_bytes_written);
int8_t sim800_read(storage_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_bytes_to_read, uint32_t *u32PL_bytes_read);

storage_t *storage_sim800_init_static(void)
{
  storage_t *objPL_storage = (storage_t*)&objS_stor_sim800;
  objPL_storage->u32_offset              = 0;
  objPL_storage->obj_virtual_table.open  = sim800_open;
  objPL_storage->obj_virtual_table.close = sim800_close;
  objPL_storage->obj_virtual_table.read  = sim800_read;
  objPL_storage->obj_virtual_table.write = sim800_write;

  return objPL_storage;
}

#ifndef UTEST
bool storage_compare_echo(storage_sim800_t *objPL_this, char *cPL_cmd, uint32_t u32L_cmd_len);
#else
bool storage_compare_echo(const char *cPL_cmd, uint32_t u32L_cmd_len);
#endif

int8_t sim800_open(storage_t *objPL_this, const char *cPL_file_name, uint8_t u8L_mode)
{
  storage_sim800_t *objPL_sim800 = (storage_sim800_t*)objPL_this;
  usart_setup(objPL_sim800->objP_uart, objPL_sim800->e_uart_num);
  sim800_power_on();
  usart_send_string(objPL_sim800->objP_uart, "ATE1\r\n");
  delay(1000);
  usart_flush(objPL_sim800->objP_uart);

  objPL_sim800->cP_file_name = cPL_file_name;
  objPL_this->u8_mode        = u8L_mode;
  objPL_this->u32_offset     = 0;

  char cPL_str[64] = { 0 };

  snprintf(cPL_str, sizeof(cPL_str), "AT+FSDEL=%s\r\n", objPL_sim800->cP_file_name);
  usart_send_string(objPL_sim800->objP_uart, cPL_str);
  delay(100);
  usart_flush(objPL_sim800->objP_uart);

  snprintf(cPL_str, sizeof(cPL_str), "AT+FSCREATE=%s\r\n", objPL_sim800->cP_file_name);
  usart_send_string(objPL_sim800->objP_uart, cPL_str);
  delay(100);
  usart_flush(objPL_sim800->objP_uart);

  return 0;
}

int8_t sim800_close(storage_t *objPL_this)
{
  storage_sim800_t *objPL_sim800 = (storage_sim800_t*)objPL_this;
  usart_deinit(objPL_sim800->objP_uart);
  sim800_power_off();
  objPL_this->u8_mode    = 0;
  objPL_this->u32_offset = 0;
  
  usart_send_string(objPL_sim800->objP_uart, "AT+FSLS=C:\\User\\\r\n");
  delay(100);
  usart_flush(objPL_sim800->objP_uart);

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
  char cPL_buff[128]     = { 0 };
  
  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "AT+FSREAD=%s,%d,%lu,%lu\r\n",
    objPL_store->cP_file_name, u8L_mode, u32L_bytes_to_read, objPL_this->u32_offset);
  usart_send_string(objPL_store->objP_uart, cPL_buff);
  delay(1);
  usart_flush(objPL_store->objP_uart);

/*
  if (storage_compare_echo(objPL_this, cPL_buff, strlen(cPL_buff)) == false)
  {
    usart_flush(objPL_this->objP_uart);
    return 0;
  }
  */

  uint32_t u32L_idx = 0;
  uint8_t u8L_data  = 0;

  while (u32L_idx <= u32L_bytes_to_read)
  {
    usart_get_byte(objPL_store->objP_uart, &u8L_data, 10);
    u8PL_buff[u32L_idx] = u8L_data;
    ++u32L_idx;
  }

  objPL_this->u32_offset += u32L_idx;

  return 0;
}

#if 0
uint32_t storage_get_chunk(storage_sim800_t *objPL_this, uint8_t *u8PL_chunk_buf, uint32_t u32L_chunk_size)
{
  ASSERT(u8PL_chunk_buf != NULL);

  const uint8_t u8L_mode = (objPL_this->obj_stor.u32_offset == 0) ? 0 : 1;
  char cPL_buff[128]     = { 0 };
  
  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "AT+FSREAD=%s,%d,%lu,%lu\r\n",
    objPL_this->cP_file_name, u8L_mode, u32L_chunk_size, objPL_this->obj_stor.u32_offset);
  usart_send_string(objPL_this->objP_uart, cPL_buff);

  if (storage_compare_echo(objPL_this, cPL_buff, strlen(cPL_buff)) == false)
  {
    usart_flush(objPL_this->objP_uart);
    return 0;
  }

  uint32_t u32L_idx = 0;
  uint8_t u8L_data  = 0;

  while (u32L_idx <= u32L_chunk_size)
  {
    usart_get_byte(objPL_this->objP_uart, &u8L_data, 10);
    u8PL_chunk_buf[u32L_idx] = u8L_data;
    ++u32L_idx;
  }

  return u32L_idx;
}

int8_t storage_get_file_size(storage_sim800_t *objPL_this, const char *cPL_file_name, uint32_t* u32PL_file_size)
{
  ASSERT(cPL_file_name != NULL);

  const char *cPL_cmd = "AT+FSFLSIZE=";
  char cPL_buff[128]  = { 0 };

  ASSERT(strlen(cPL_file_name) < (ARRAY_SIZE(cPL_buff) - strlen(cPL_cmd) + 1));

  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "%s%s\r\n+FSFLSIZE: ", cPL_cmd, cPL_file_name);
  usart_send_string(objPL_this->objP_uart, cPL_buff);

  delay(100);

  if (storage_compare_echo(objPL_this, cPL_buff, strlen(cPL_buff)) == false)
  {
    usart_flush(objPL_this->objP_uart);
    return eStorageError;
  }

  uint32_t u32L_idx = 0;
  uint8_t u8L_byte  = 0;
  // Get file size
  while (usart_get_byte(objPL_this->objP_uart, &u8L_byte, 100))
  {
    cPL_buff[u32L_idx] = (char)u8L_byte;

    ++u32L_idx;

    if (u32L_idx >= ARRAY_SIZE(cPL_buff))
    {
      ASSERT(0);
      usart_flush(objPL_this->objP_uart);
      return eStorageError;
    }
  }

  cPL_buff[u32L_idx] = '\0';
  *u32PL_file_size   = (uint32_t)atoi(cPL_buff);
  
  return eStorageOk;
}

bool storage_compare_echo(storage_sim800_t *objPL_this, char *cPL_cmd, uint32_t u32L_cmd_len)
{
  ASSERT(cPL_cmd != NULL);
  ASSERT(cPL_cmd[0] != '\0');
  ASSERT(u32L_cmd_len > 0);

  bool bL_return    = false;
  uint32_t u32L_idx = 0;
  char cL_input     = '\0';

  while (usart_get_byte(objPL_this->objP_uart, (uint8_t*)&cL_input, 100))
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

bool storage_sim800_wait_response(storage_sim800_t *objPL_this, char **cPL_expected, uint8_t u8L_ans_num, uint32_t u32L_timeout)
{
  bool bL_return    = false;
  uint32_t u32L_idx = 0;
  char cL_input     = '\0';

  while (usart_get_byte(objPL_this->objP_uart, (uint8_t*)&cL_input, u32L_timeout))
  {

  }

  return bL_return;
}
#endif
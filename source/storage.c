/**
 * @file storage.c 
 */

#include <stdint.h>
#include <stddef.h>

#include "system/assert.h"
#include "utilities/toolbox.h"

#include "storage.h"

int8_t storage_open(storage_t *objPL_this, const char *cPL_file_name, uint8_t u8L_mode)
{
  ASSERT(objPL_this);
  ASSERT(objPL_this->obj_virtual_table.open);
  return objPL_this->obj_virtual_table.open(objPL_this, cPL_file_name, u8L_mode);
}

int8_t storage_close(storage_t *objPL_this)
{
  ASSERT(objPL_this);
  ASSERT(objPL_this->obj_virtual_table.close);
  return objPL_this->obj_virtual_table.close(objPL_this);
}

int8_t storage_write(storage_t *objPL_this, const uint8_t *u8PL_buff, uint32_t u32L_buff_size, uint32_t *u32PL_bytes_written)
{
  ASSERT(objPL_this);
  ASSERT(objPL_this->obj_virtual_table.write);
  return objPL_this->obj_virtual_table.write(objPL_this, u8PL_buff, u32L_buff_size, u32PL_bytes_written);
}

int8_t storage_read(storage_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_bytes_to_read, uint32_t *u32PL_bytes_read)
{
  ASSERT(objPL_this);
  ASSERT(objPL_this->obj_virtual_table.read);
  return objPL_this->obj_virtual_table.read(objPL_this, u8PL_buff, u32L_bytes_to_read, u32PL_bytes_read);
}

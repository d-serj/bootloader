/**
 * @file storage_internal.c
 */

#include <stdint.h>

#include "drivers/flash.h"
#include "system/assert.h"
#include "utilities/toolbox.h"

#include "storage.h"
#include "storage_internal.h"

typedef struct
{
  storage_t obj_vft;
} storage_internal_t;

static storage_internal_t objS_strg_internal;

static int8_t storage_internal_open(storage_t *objPL_this, const char *cPL_file_name, uint8_t u8L_mode);
static int8_t storage_internal_close(storage_t *objPL_this);
static int8_t storage_internal_write(storage_t *objPL_this,
  const uint8_t *u8PL_buff, uint32_t u32L_buff_size, uint32_t *u32PL_bytes_written);
static int8_t storage_internal_read(storage_t *objPL_this,
  uint8_t *u8PL_buff, uint32_t u32L_bytes_to_read, uint32_t *u32PL_bytes_read);

storage_t *storage_internal_init_static(void)
{
  storage_t *objPL_storage = (storage_t*)&objS_strg_internal;
  objPL_storage->u32_offset              = 0;
  objPL_storage->obj_virtual_table.open  = storage_internal_open;
  objPL_storage->obj_virtual_table.close = storage_internal_close;
  objPL_storage->obj_virtual_table.read  = storage_internal_read;
  objPL_storage->obj_virtual_table.write = storage_internal_write;

  return objPL_storage;
}

static int8_t storage_internal_open(storage_t *objPL_this, const char *cPL_file_name, uint8_t u8L_mode)
{
  UNUSED(objPL_this);
  UNUSED(cPL_file_name);
  objPL_this->u8_mode = u8L_mode;
  return 0;
}

static int8_t storage_internal_close(storage_t *objPL_this)
{
  UNUSED(objPL_this);
  objPL_this->u8_mode    = 0;
  objPL_this->u32_offset = 0;
  return 0;
}

static int8_t storage_internal_write(storage_t *objPL_this,
  const uint8_t *u8PL_buff, uint32_t u32L_buff_size, uint32_t *u32PL_bytes_written)
{
  if (objPL_this->u8_mode != eStorageModeWrite)
  {
    ASSERT(0);
    return -1;
  }

  const int8_t s8L_ret = flash_program_data(objPL_this->u32_offset, u8PL_buff, u32L_buff_size);
  if (s8L_ret == 0)
  {
    objPL_this->u32_offset += u32L_buff_size;

    if (u32PL_bytes_written)
    {
      *u32PL_bytes_written = u32L_buff_size;
    }
  }

  return s8L_ret;
}

static int8_t storage_internal_read(storage_t *objPL_this,
  uint8_t *u8PL_buff, uint32_t u32L_bytes_to_read, uint32_t *u32PL_bytes_read)
{
  if (objPL_this->u8_mode != eStorageModeRead)
  {
    ASSERT(0);
    return -1;
  }

  flash_read_data(objPL_this->u32_offset, u32L_bytes_to_read, u8PL_buff);
  objPL_this->u32_offset += u32L_bytes_to_read;

  if (u32PL_bytes_read)
  {
    *u32PL_bytes_read = u32L_bytes_to_read;
  }

  return 0;
}

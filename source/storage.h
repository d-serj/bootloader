/**
 * @file storage.h
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>

enum eStorageMode
{
  eStorageModeRead  = 1,
  eStorageModeWrite = 2,
};

typedef struct storage storage_t;

typedef struct
{
  int8_t (*open)(storage_t *objPL_this, const char *cPL_file_name, uint8_t u8L_mode);
  int8_t (*close)(storage_t *objPL_this);
  int8_t (*write)(storage_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_buff_size, uint32_t *u32PL_bytes_written);
  int8_t (*read)(storage_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_bytes_to_read, uint32_t *u32PL_bytes_read);
} storage_virtual_table_t;

struct storage
{
  storage_virtual_table_t obj_virtual_table;  ///< Virtual functions
  uint32_t u32_offset;                        ///< Read/Write offset
  uint8_t  u8_mode;                           ///< File mode from eStorageMode
};

#endif // STORAGE_H_

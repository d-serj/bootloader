/**
 * @file image.c
 * @brief Image related funcitons
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include <system/assert.h>

#include <utilities/toolbox.h>
#include <utilities/crc32.h>
#include <delay.h>


#include "storage/storage_sim800.h"
#include "img-header.h"
#include "image.h"

// STM32F105 has flash banks with size 2048 bytes each
static uint8_t u8PS_buff[2048] = { 0 };

#ifndef UTEST
static inline bool image_header_check(const image_hdr_t *objPL_img_hdr) __attribute__((always_inline));
static bool image_compare_crc32(image_t *objPL_this, uint32_t u32L_crc32);
#else
bool image_header_check(const image_hdr_t *objPL_img_hdr);
bool image_compare_crc32(image_t *objPL_this);
#endif // UTEST

int8_t image_open(image_t *objPL_this, storage_t *objPL_storage, const char *cPL_filename)
{
  objPL_this->objP_storage = objPL_storage;
  objPL_this->cP_file_name = cPL_filename;
  objPL_this->obj_img_hdr  = (image_hdr_t){ 0 };

  // Get image header from storage
  uint8_t u8PL_buff[sizeof(image_hdr_t)] = { 0 };
  
  int8_t s8L_ret = storage_open(objPL_storage, cPL_filename, eStorageModeRead);
  if (s8L_ret != eStorageOk)
  {
    return -1;
  }

  s8L_ret = storage_read(objPL_storage, u8PL_buff, sizeof(image_hdr_t), NULL);
  if (s8L_ret != eStorageOk)
  {
    return -1;
  }

  memcpy(&objPL_this->obj_img_hdr, u8PL_buff, sizeof(image_hdr_t));

  if (image_header_check(&objPL_this->obj_img_hdr) == false)
  {
    return -1;
  }

  objPL_this->u32_file_size = objPL_this->obj_img_hdr.u32_data_size + sizeof(image_hdr_t);

  return 0;
}

int8_t image_close(image_t *objPL_this)
{
  objPL_this->cP_file_name = NULL;
  objPL_this->obj_img_hdr  = (image_hdr_t){ 0 };
  
  const int8_t s8L_ret = storage_close(objPL_this->objP_storage);
  ASSERT(s8L_ret == eStorageOk);
  objPL_this->objP_storage  = NULL;
  objPL_this->u32_file_size = 0;

  return s8L_ret;
}

bool image_header_check(const image_hdr_t *objPL_img_hdr)
{
  return ((objPL_img_hdr->u16_image_magic == IMAGE_MAGIC)
    && (objPL_img_hdr->u16_image_hdr_version == IMAGE_VERSION_CURRENT));
}

bool image_validate(image_t *objPL_this)
{
  ASSERT(objPL_this != NULL);

  if (image_header_check(&objPL_this->obj_img_hdr) == false)
  {
    return false;
  }

  return image_compare_crc32(objPL_this, objPL_this->obj_img_hdr.u32_crc);
}

bool image_compare_crc32(image_t *objPL_this, uint32_t u32L_crc32)
{
  const uint32_t u32L_buff_size  = ARRAY_SIZE(u8PS_buff);
  uint32_t u32L_file_crc32       = 0;
  uint32_t u32L_bytes_read       = 0;
  int8_t   s8L_ret               = eStorageOk;
  storage_t *objPL_storage       = objPL_this->objP_storage;
  uint32_t u32L_rest_of_firmware = objPL_this->u32_file_size - sizeof(image_hdr_t);

  delay(5);

  while ((s8L_ret == eStorageOk) && u32L_rest_of_firmware)
  {
    const uint32_t u32L_bytes_to_read = MIN(u32L_buff_size, u32L_rest_of_firmware);
    s8L_ret = storage_read(objPL_storage, u8PS_buff, u32L_bytes_to_read, &u32L_bytes_read);
    ASSERT(u32L_bytes_read == u32L_bytes_to_read);

    u32L_file_crc32        = crc32((const void*)u8PS_buff, u32L_bytes_read, u32L_file_crc32);
    u32L_rest_of_firmware -= u32L_bytes_read;

    delay(5);
  }

  s8L_ret = storage_close(objPL_storage);
  ASSERT(s8L_ret == eStorageOk);

  return (u32L_file_crc32 == u32L_crc32);
}

int8_t image_copy(image_t *objPL_this, storage_t *objPL_dest)
{
  storage_open(objPL_this->objP_storage, objPL_this->cP_file_name, eStorageModeRead);
  storage_open(objPL_dest, objPL_this->cP_file_name, eStorageModeWrite);

  const uint32_t u32L_buff_size = ARRAY_SIZE(u8PS_buff);
  uint32_t u32L_bytes_read      = 0;
  int8_t   s8L_ret              = 0;
  uint32_t u32L_bytes_written   = 0;
  uint32_t u32L_rest_of_file    = objPL_this->u32_file_size;

  while ((s8L_ret == eStorageOk) && u32L_rest_of_file)
  {
    const uint32_t u32L_bytes_to_read = MIN(u32L_buff_size, u32L_rest_of_file);
    s8L_ret  = storage_read(objPL_this->objP_storage, u8PS_buff, u32L_bytes_to_read, &u32L_bytes_read);
    ASSERT(u32L_bytes_read > 0);

    s8L_ret |= storage_write(objPL_dest, u8PS_buff, u32L_bytes_read, &u32L_bytes_written);
    ASSERT(u32L_bytes_written > 0);
    ASSERT(u32L_bytes_written == u32L_bytes_read);

    u32L_rest_of_file -= u32L_bytes_written;
  }

  storage_close(objPL_dest);

  return s8L_ret;
}

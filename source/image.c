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

#include "crc32.h"

#include "img-header.h"
#include "image.h"
#include "storage_sim800.h"

// STM32F105 has flash banks with size 2048 bytes each
static uint8_t u8PS_buff[2048] = { 0 };

#ifndef UTEST
static inline bool image_header_check(const image_hdr_t *objPL_img_hdr) __attribute__((always_inline));
static bool image_compare_crc32(image_t *objPL_this);
#else
bool image_header_check(const image_hdr_t *objPL_img_hdr);
bool image_compare_crc32(image_t *objPL_this);
#endif // UTEST

int8_t image_open(image_t *objPL_this, storage_t *objPL_storage, const char *cPL_filename)
{
  objPL_this->objP_storage = objPL_storage;
  objPL_this->cP_file_name = cPL_filename;
  objPL_this->obj_img_hdr  = (image_hdr_t){ 0 };

  uint32_t u32L_file_size = 0;

  objPL_this->u32_file_size     = u32L_file_size;
  objPL_this->u32_firmware_size = objPL_this->u32_file_size - sizeof(image_hdr_t);

  // Get image header from storage
  uint8_t u8PL_buff[sizeof(image_hdr_t)] = { 0 };
  storage_open(objPL_storage, cPL_filename, eStorageModeRead);
  storage_read(objPL_storage, u8PL_buff, sizeof(image_hdr_t), NULL);
  memcpy(&objPL_this->obj_img_hdr, u8PL_buff, sizeof(image_hdr_t));
  storage_close(objPL_storage);

  if (image_header_check(&objPL_this->obj_img_hdr) == false)
  {
    return -1;
  }

  return 0;
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
  
  // Compare stored in the header image size with size stored in memory
  if (objPL_this->u32_firmware_size != objPL_this->obj_img_hdr.u32_data_size)
  {
    return false;
  }

  return image_compare_crc32(objPL_this);
}

bool image_compare_crc32(image_t *objPL_this)
{
  uint32_t u32L_file_crc32 = 0;
  uint32_t u32L_bytes_read = 0;
  int8_t s8L_ret           = eStorageOk;
  storage_t *objPL_storage = objPL_this->objP_storage;

  s8L_ret = storage_open(objPL_storage, objPL_this->cP_file_name, eStorageModeRead);
  ASSERT(s8L_ret == eStorageOk);

  do
  {
    s8L_ret = storage_read(objPL_storage, u8PS_buff, ARRAY_SIZE(u8PS_buff), &u32L_bytes_read);
    ASSERT(s8L_ret != eStorageError);
    u32L_file_crc32 = crc32((const void*)u8PS_buff, u32L_bytes_read, u32L_file_crc32);
  }
  while ((s8L_ret != eStorageError) && (u32L_bytes_read > 0));

  s8L_ret = storage_close(objPL_storage);
  ASSERT(s8L_ret == eStorageOk);

  return (u32L_file_crc32 == objPL_this->obj_img_hdr.u32_crc);
}

int8_t image_copy(image_t *objPL_this, storage_t *objPL_source, storage_t *objPL_dest)
{
  storage_open(objPL_source, objPL_this->cP_file_name, eStorageModeRead);
  storage_open(objPL_dest, objPL_this->cP_file_name, eStorageModeWrite);

  uint32_t u32L_bytes_read    = 0;
  int8_t s8L_ret              = 0;
  uint32_t u32L_bytes_written = 0;

  while ((s8L_ret == 0) && (u32L_bytes_read > 0))
  {
    s8L_ret  = storage_read(objPL_source, u8PS_buff, ARRAY_SIZE(u8PS_buff), &u32L_bytes_read);
    s8L_ret |= storage_write(objPL_dest, u8PS_buff, u32L_bytes_read, &u32L_bytes_written);
    ASSERT(u32L_bytes_written == u32L_bytes_read);
  }

  storage_close(objPL_source);
  storage_close(objPL_dest);

  return s8L_ret;
}

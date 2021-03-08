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
#include "storage.h"

#ifndef UTEST
static void image_header_get(image_t *objPL_this);
static inline bool image_header_check(const image_hdr_t *objPL_img_hdr) __attribute__((always_inline));
static bool image_compare_crc32(image_t *objPL_this);
#else
void image_header_get(image_t *objPL_this);
bool image_header_check(const image_hdr_t *objPL_img_hdr);
bool image_compare_crc32(image_t *objPL_this);
#endif // UTEST

void image_init(image_t *objPL_this, const char *cPL_filename)
{
  objPL_this->cP_file_name    = cPL_filename;
  objPL_this->obj_img_hdr     = (image_hdr_t){ 0 };
  objPL_this->u32_read_offset = 0;

  objPL_this->u32_file_size     = storage_get_file_size(objPL_this->cP_file_name);
  objPL_this->u32_firmware_size = objPL_this->u32_file_size - sizeof(image_hdr_t);
}

bool image_header_check(const image_hdr_t *objPL_img_hdr)
{
  return ((objPL_img_hdr->u16_image_magic == IMAGE_MAGIC)
    && (objPL_img_hdr->u16_image_hdr_version == IMAGE_VERSION_CURRENT));
}

void image_header_get(image_t *objPL_this)
{
  // Get image header from storage
  uint8_t u8PL_buff[sizeof(image_hdr_t)] = { 0 };
  storage_get_chunk(objPL_this->cP_file_name, 0, u8PL_buff, sizeof(image_hdr_t));
  memcpy(&objPL_this->obj_img_hdr, u8PL_buff, sizeof(image_hdr_t));
  // Set read offset
  objPL_this->u32_read_offset = sizeof(image_hdr_t);
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
  uint8_t u8PL_buff[256]   = { 0 };
  uint32_t u32L_file_crc32 = 0;
  uint32_t u32L_bytes_read = 0;

  while ((u32L_bytes_read = image_read(objPL_this, u8PL_buff, ARRAY_SIZE(u8PL_buff))) != 0)
  {
    u32L_file_crc32 = crc32((const void*)u8PL_buff, u32L_bytes_read, u32L_file_crc32);
  }

  return (u32L_file_crc32 == objPL_this->obj_img_hdr.u32_crc);
}

uint32_t image_read(image_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_buff_size)
{
  const uint32_t u32L_remaining_bytes =
    objPL_this->u32_read_offset - objPL_this->u32_firmware_size;

  if (u32L_remaining_bytes == 0)
  {
    return 0;
  }

  const uint32_t u32L_size_to_get = 
    (u32L_remaining_bytes > u32L_buff_size) ? u32L_buff_size : u32L_remaining_bytes;
  const uint32_t u32L_num_bytes_read = 
    storage_get_chunk(objPL_this->cP_file_name, objPL_this->u32_read_offset, u8PL_buff, u32L_size_to_get);
  objPL_this->u32_read_offset += u32L_size_to_get;

  return u32L_num_bytes_read;
}

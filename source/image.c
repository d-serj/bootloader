/**
 * @file image.c
 * @brief Image related funcitons
 */

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include <modem/modem.h>

#include "crc32.h"

#include "img-header.h"
#include "image.h"
#include "storage.h"

void image_get_chunk(uint32_t u32L_chunk_addr, uint8_t* u8PL_chunk_buf, uint32_t u32L_chunk_size);
static inline bool image_header_check(const image_hdr_t *objPL_img_hdr) __attribute__((always_inline));
static uint32_t image_get_size(void);

static inline bool image_header_check(const image_hdr_t *objPL_img_hdr)
{
  return ((objPL_img_hdr->u16_image_magic == IMAGE_MAGIC)
    && (objPL_img_hdr->u16_image_hdr_version == IMAGE_VERSION_CURRENT));
}

const image_hdr_t image_header_get(void)
{
  // Get image header from SIM800
  image_hdr_t objL_image_header          = { 0 };
  uint8_t u8PL_buff[sizeof(image_hdr_t)] = { 0 };
  image_get_chunk(0, u8PL_buff, sizeof(image_hdr_t));
  memcpy(&objL_image_header, u8PL_buff, sizeof(image_hdr_t));

  return objL_image_header;
}

bool image_validate(const image_hdr_t *objPL_hdr)
{
  assert(objPL_hdr != NULL);
  // Validate header
  if (image_header_check(objPL_hdr) == false)
  {
    return false;
  }
  // Compare stored in the header image size with size stored in memory
  if (image_get_size() != objPL_hdr->u32_data_size)
  {
    return false;
  }

  const uint32_t u32L_chunk_size = 256u;
  uint32_t u32L_file_crc32       = 0;
  uint32_t u32L_remaining_bytes  = objPL_hdr->u32_data_size;
  // Skip header since it is already downloaded
  uint32_t u32L_download_pos     = sizeof(image_hdr_t);

  while (u32L_remaining_bytes > 0)
  {
    const uint32_t u32L_size_to_get =
      (u32L_remaining_bytes > u32L_chunk_size) ? u32L_chunk_size : u32L_remaining_bytes;
    uint8_t u8PL_chunk[u32L_size_to_get];
    memset(u8PL_chunk, 0, u32L_size_to_get);
    image_get_chunk(u32L_download_pos, u8PL_chunk, u32L_size_to_get);

    u32L_file_crc32 = crc32((const void*)u8PL_chunk, u32L_size_to_get, u32L_file_crc32);

    u32L_download_pos    += u32L_size_to_get;
    u32L_remaining_bytes -= u32L_size_to_get;
  }

  return (u32L_file_crc32 == objPL_hdr->u32_crc);
}

void image_get_chunk(uint32_t u32L_chunk_addr, uint8_t* u8PL_chunk_buf, uint32_t u32L_chunk_size)
{
  assert(u8PL_chunk_buf != NULL);

  storage_get_chunk(NULL, u32L_chunk_addr, u8PL_chunk_buf, u32L_chunk_size);
}

uint32_t image_get_size(void)
{
  // TMP
  return storage_get_file_size(NULL);
}

uint32_t image_test(uint16_t u16L_a, uint16_t u16L_b)
{
  return u16L_a + u16L_b;
}

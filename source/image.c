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
#include "firmware.h"

#include "img-header.h"
#include "image.h"

static const uint8_t *image_get_chunk(uint32_t u32L_chunk_addr, uint32_t u32L_chunk_size);
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
  image_hdr_t objL_image_header = { 0 };
  const uint8_t* u8PL_hdr = image_get_chunk(0, sizeof(image_hdr_t));
  memcpy(&objL_image_header, u8PL_hdr, sizeof(image_hdr_t));

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
    const uint8_t *u8PL_chunk = image_get_chunk(u32L_download_pos, u32L_size_to_get);

    if (u8PL_chunk == NULL)
    {
      break;
    }

    u32L_file_crc32 = crc32((const void*)u8PL_chunk, u32L_size_to_get, u32L_file_crc32);

    u32L_download_pos    += u32L_size_to_get;
    u32L_remaining_bytes -= u32L_size_to_get;
  }

  return (u32L_file_crc32 == objPL_hdr->u32_crc);
}

const uint8_t *image_get_chunk(uint32_t u32L_chunk_addr, uint32_t u32L_chunk_size)
{
  assert(u32L_chunk_addr < u32_firmware_size);
  assert(u32L_chunk_size <= (u32_firmware_size - u32L_chunk_addr));

  return &u8P_firmware_bin[u32L_chunk_addr];
}

uint32_t image_get_size(void)
{
  // TMP
  return u32_firmware_size - sizeof(image_hdr_t);
}

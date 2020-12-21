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

#include "image.h"

static uint8_t *image_get_chunk(uint32_t u32L_chunk_addr, uint32_t u32L_chunk_size);


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

  if (image_header_check(objPL_hdr) == false)
  {
    return false;
  }

  const uint32_t u32L_chunk_size = 256u;
  uint32_t u32L_file_crc32       = 0;
  uint32_t u32L_remaining_bytes  = objPL_hdr->u32_data_size;
  uint32_t u32L_download_pos     = 0;

  while (u32L_remaining_bytes > 0)
  {
    const uint32_t u32L_size_to_get =
      (u32L_remaining_bytes > u32L_chunk_size) ? u32L_chunk_size : u32L_remaining_bytes;
      /// TODO odd length
    const uint8_t *u8PL_chunk = image_get_chunk(u32L_download_pos, u32L_size_to_get);

    if (u8PL_chunk == NULL)
    {
      break;
    }

    u32L_file_crc32 = crc32((const void*)u8PL_chunk, u32L_size_to_get, u32L_file_crc32);

    u32L_remaining_bytes -= u32L_size_to_get;
  }

  return (u32L_file_crc32 == objPL_hdr->u32_crc);
}

uint8_t *image_get_chunk(uint32_t u32L_chunk_addr, uint32_t u32L_chunk_size)
{
  return NULL; 
}

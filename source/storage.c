/**
 * @file storage.c
 * @brief Functions to work with image storage. Implementation
 */

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "img-header.h"
#include "firmware.h"
#include "storage.h"

void storage_get_chunk(const char *cPL_file_name, uint32_t u32L_chunk_addr, uint8_t *u8PL_chunk_buf, uint32_t u32L_chunk_size)
{
  assert(u32L_chunk_addr < u32_firmware_size);
  assert(u32L_chunk_size <= (u32_firmware_size - u32L_chunk_addr));

  memcpy(u8PL_chunk_buf, &u8P_firmware_bin[u32L_chunk_addr], u32L_chunk_size);
}

uint32_t storage_get_file_size(const char *cPL_file_name)
{
  return u32_firmware_size - sizeof(image_hdr_t);
}

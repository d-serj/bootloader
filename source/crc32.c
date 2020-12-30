/**
 * @file crc32.c
 */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "crc32.h"

#define CRC32_POLYNOME  (0xEDB88320L)

static uint32_t crc32_for_byte(uint32_t r)
{
  for(int j = 0; j < 8; ++j)
  {
    r = (r & 1? 0: (uint32_t)CRC32_POLYNOME) ^ r >> 1;
  }

  return r ^ (uint32_t)0xFF000000L;
}

uint32_t crc32(const void *data, size_t n_bytes, uint32_t crc_in)
{
  static uint32_t table[0x100];

  assert(data != NULL);

  if(!*table)
  {
    for(size_t i = 0; i < 0x100; ++i)
    {
      table[i] = crc32_for_byte(i);
    }
  }

  uint32_t crc = crc_in;

  for(size_t i = 0; i < n_bytes; ++i)
  {
    crc = table[(uint8_t)crc ^ ((uint8_t*)data)[i]] ^ crc >> 8;
  }

  return crc;
}
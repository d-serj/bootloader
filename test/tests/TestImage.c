/**
 * @file TestImage.c
 * @brief Unit tests for image.c/h
 */

#include <assert.h>
#include <string.h>

#include <image.h>
#include <unity.h>
#include <firmware.h>

#include "TestImage.h"

void setUp(void)
{
  
}

void tearDown(void)
{
  
}

uint32_t storage_get_file_size(const char* cPL_file_name)
{
  return u32_firmware_size;
}

void storage_get_chunk(const char *cPL_file_name, uint32_t u32L_chunk_addr, uint8_t *u8PL_chunk_buf, uint32_t u32L_chunk_size)
{
  assert(u32L_chunk_addr < u32_firmware_size);
  assert(u32L_chunk_size <= (u32_firmware_size - u32L_chunk_addr));

  memcpy(u8PL_chunk_buf, &u8P_firmware_bin[u32L_chunk_addr], u32L_chunk_size);
}

void test_ImageImageSize(void)
{
  const image_hdr_t objL_hdr = image_header_get();
  /* All of these should pass */
  TEST_ASSERT_EQUAL(image_get_size() - sizeof(image_hdr_t), objL_hdr.u32_data_size);
}

void test_ImageHeaderCheck(void)
{
  const image_hdr_t objL_hdr = image_header_get();
  /* All of these should pass */
  TEST_ASSERT_TRUE(image_header_check(&objL_hdr));
}

void test_ImageValidateTest(void)
{
  const image_hdr_t objL_hdr = image_header_get();
  /* All of these should pass */
  TEST_ASSERT_TRUE(image_validate(&objL_hdr));
}

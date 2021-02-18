/**
 * @file TestImage.c
 * @brief Unit tests for image.c/h
 */

#include <stdint.h>
#include <assert.h>
#include <string.h>

#include <unity.h>
#include <fff.h>

#include <storage.h>
#include <image.h>

#include "test_image.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(usart_send_blocking, uint32_t, uint16_t);
FAKE_VALUE_FUNC(uint16_t, usart_recv, uint32_t);

void setUp(void)
{
  storage_init();
}

void tearDown(void)
{
  
}

void test_ImageImageSize(void)
{
  const image_hdr_t objL_hdr = image_header_get();
  TEST_ASSERT_EQUAL(image_get_size(), objL_hdr.u32_data_size);
}

void test_ImageHeaderCheck(void)
{
  const image_hdr_t objL_hdr = image_header_get();
  TEST_ASSERT_TRUE(image_header_check(&objL_hdr));
}

void test_ImageValidateTest(void)
{
  const image_hdr_t objL_hdr = image_header_get();
  //TEST_ASSERT_TRUE(image_validate(&objL_hdr));
}

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
#include <utilities/toolbox.h>

#include "test_image.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(usart_send_blocking, uint32_t, uint16_t);

char cPL_buff[256]       = { 0 };
static uint16_t s16S_idx = 0;

uint16_t usart_recv(uint32_t usart)
{
  return (uint16_t)cPL_buff[s16S_idx++];
}

void setUp(void)
{
  storage_init();
  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "AT+FSREAD=%s,%d,%d,%d\r\n", 
    "firmware.bin", 0, sizeof(image_hdr_t), 0);
}

void tearDown(void)
{
  s16S_idx = 0;
}

void test_ImageImageSize(void)
{
  RESET_FAKE(usart_send_blocking);
  const image_hdr_t objL_hdr = image_header_get();
  TEST_ASSERT_EQUAL(strlen(cPL_buff), usart_send_blocking_fake.call_count);
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
  TEST_ASSERT_TRUE(image_validate(&objL_hdr));
}

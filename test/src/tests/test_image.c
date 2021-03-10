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

extern bool image_header_check(const image_hdr_t *objPL_img_hdr);
extern bool image_compare_crc32(image_t *objPL_this);

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(usart_send_blocking, uint32_t, uint16_t);


void setUp(void)
{
  
}

void tearDown(void)
{
  
}

void test_image_init(void)
{
  image_t objL_image = (image_t){ 0 };

  image_init(&objL_image, "firmware.bin");

  TEST_ASSERT_EQUAL(objL_image.u32_firmware_size, objL_image.obj_img_hdr.u32_data_size);
}

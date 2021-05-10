/**
 * @file TestImage.c
 * @brief Unit tests for image.c/h
 */

#include <stdint.h>
#include <assert.h>
#include <string.h>

#include <unity.h>
#include <fff.h>

#include <firmware.h>

#include <storage.h>
#include <image.h>
#include <utilities/toolbox.h>

extern bool image_header_check(const image_hdr_t *objPL_img_hdr);
extern bool image_compare_crc32(image_t *objPL_this);

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(storage_init);
FAKE_VALUE_FUNC(int8_t, storage_get_file_size, const char*, uint32_t*);
FAKE_VALUE_FUNC(uint32_t, storage_get_chunk, const char*, uint32_t, uint8_t*, uint32_t);

void setUp(void)
{
  RESET_FAKE(storage_init);
  RESET_FAKE(storage_get_file_size);
  RESET_FAKE(storage_get_chunk);
}

void tearDown(void)
{

}

/*
void test_image_init(void)
{
  image_t objL_image = (image_t){ 0 };

  storage_get_file_size_fake.return_val = 100;

  image_init(&objL_image, "firmware.bin");

  TEST_ASSERT_EQUAL_UINT32(objL_image.u32_file_size, storage_get_file_size_fake.return_val);
}
*/
/*
void test_image_read(void)
{
  image_t objL_image = (image_t){ 0 };

  storage_get_chunk_fake = (const uint8_t*)u8P_firmware_bin;

  image_header_get(&objL_image);

  TEST_ASSERT_EQUAL(10, objL_image.obj_img_hdr.u32_data_size);
}
*/

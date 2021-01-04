/**
 * @file TestImage.c
 * @brief Unit tests for image.c/h
 */

#include <image.h>

#include "unity.h"

void setUp(void)
{
  
}

void tearDown(void)
{
  
}

void test_ImageTest(void)
{
  const image_hdr_t objL_hdr = image_header_get();
  /* All of these should pass */
  TEST_ASSERT_TRUE(image_validate(&objL_hdr));
}

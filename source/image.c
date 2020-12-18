/**
 * @file image.c
 * @brief Image related funcitons
 */

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "crc32.h"

#include "image.h"


static image_hdr_t objL_image_header = { 0 };

static uint32_t image_calc_crc32(const image_part_t *objPL_img_part);


const image_hdr_t *image_header_get(void)
{
  // Get image header from SIM800
  // memcpy image header to objL_image_header
  return &objL_image_header;
}

bool image_validate(image_part_t *objPL_img_part)
{
  assert(objPL_img_part != NULL);

  objPL_img_part->u32_crc32 = image_calc_crc32(objPL_img_part);

  return true;
}

static uint32_t image_calc_crc32(const image_part_t *objPL_img_part)
{
  return crc32(objPL_img_part->u8P_data,
    objPL_img_part->u32_data_size,
    objPL_img_part->u32_crc32);
}

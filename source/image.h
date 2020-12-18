/**
 * @file image.h
 * @brief Image related funcitons
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdbool.h>

#include "img-header.h"

struct Image_part
{
  const uint8_t *u8P_data;
  uint32_t u32_data_size;
  uint32_t u32_crc32;
};
typedef struct Image_part image_part_t;

bool image_validate(image_part_t *objPL_img);

#endif // IMAGE_H_

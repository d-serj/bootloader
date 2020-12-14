/**
 * @file image.h
 * @brief Image related funcitons
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdbool.h>

#include "img-header.h"

struct Image
{
  const image_hdr_t *objP_img_hdr;
  const uint8_t *u8P_data;
};
typedef struct Image image_t;

bool image_validate(const image_t *objPL_img);

#endif // IMAGE_H_

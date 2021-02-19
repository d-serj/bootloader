/**
 * @file image.h
 * @brief Image related funcitons
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>
#include <stdbool.h>

#include "img-header.h"

typedef struct
{
  image_hdr_t obj_img_hdr;
  const char *cP_file_name;
  uint32_t u32_read_offset;
  uint32_t u32_file_size;
  uint32_t u32_firmware_size;
} image_t;
 
void image_init(image_t *objPL_this, const char *cPL_filename);

uint32_t image_read(image_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_buff_size);

bool image_validate(image_t *objPL_this);

#endif // IMAGE_H_

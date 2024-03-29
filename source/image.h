/**
 * @file image.h
 * @brief Image related funcitons
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>
#include <stdbool.h>

#include "storage/storage.h"
#include "img-header.h"

#define IMAGE_NAME  ("firmware.bin")

/** Image struct */
typedef struct
{
  image_hdr_t obj_img_hdr;      ///< Image header
  storage_t *objP_storage; 
  const char *cP_file_name;     ///< File name
  uint32_t   u32_file_size;     ///< File size on the storage
} image_t;

int8_t image_open(image_t *objPL_this, storage_t *objPL_storage, const char *cPL_filename);

int8_t image_close(image_t *objPL_this);

int8_t image_copy(image_t *objPL_this, storage_t *objPL_dest);

/**
 * @brief Validate image
 * @param objPL_this .. pointer to image instance
 * @return true if image is correct, false otherwise
 */
bool image_validate(image_t *objPL_this);

#endif // IMAGE_H_

/**
 * @file image.h
 * @brief Image related funcitons
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>
#include <stdbool.h>

#include "img-header.h"

/** Image struct */
typedef struct
{
  image_hdr_t obj_img_hdr;      ///< Image header
  const char *cP_file_name;     ///< File name
  uint32_t   u32_read_offset;   ///< Read offset
  uint32_t   u32_file_size;     ///< File size on the storage
  uint32_t   u32_firmware_size; ///< The size of firmware part
} image_t;
 
int8_t image_open(image_t *objPL_this, const char *cPL_filename);

/**
 * @brief Read image data from the storage
 * @param objPL_this .. pointer to image instance
 * @param u8PL_buff .. pointer to buffer where data is going to be stored
 * @param u32L_buff_size .. buffer size
 * @return uint32_t number of bytes that have been read
 */
uint32_t image_read(image_t *objPL_this, uint8_t *u8PL_buff, uint32_t u32L_buff_size);

/**
 * @brief Get image header
 * @param objPL_this .. pointer to image instance
 */
void image_header_get(image_t *objPL_this);

/**
 * @brief Validate image
 * @param objPL_this .. pointer to image instance
 * @return true if image is correct, false otherwise
 */
bool image_validate(image_t *objPL_this);

#endif // IMAGE_H_

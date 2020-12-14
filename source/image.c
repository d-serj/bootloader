/**
 * @file image.c
 * @brief Image related funcitons
 */

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "crc32.h"
#include "img-header.h"

#include "image.h"


static inline bool image_check_header(const image_hdr_t *objPL_img);
static bool image_check_crc32(const image_hdr_t *objPL_img);


bool image_validare(const image_hdr_t *objPL_img)
{
  assert(objPL_img != NULL);

  return (image_check_header(objPL_img) == false)
    && image_check_crc32(objPL_img);
}

static inline bool image_check_header(const image_hdr_t *objPL_img)
{
  return (objPL_img->u16_image_magic == IMAGE_MAGIC);
}

static bool image_check_crc32(const image_hdr_t *objPL_img)
{
  
}

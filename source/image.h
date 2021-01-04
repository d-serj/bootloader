/**
 * @file image.h
 * @brief Image related funcitons
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>
#include <stdbool.h>

#include "img-header.h"

const image_hdr_t image_header_get(void);

bool image_validate(const image_hdr_t *objPL_hdr);

uint32_t image_test(uint16_t u16L_a, uint16_t u16L_b);

#endif // IMAGE_H_

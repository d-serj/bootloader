/**
 * @file TestImage.h
 * @brief Shared private functions from image.c for unit tests purposes
 */

#ifndef TEST_IMAGE_H_
#define TEST_IMAGE_H_

#include <stdint.h>
#include <stdbool.h>

#include <img-header.h>

void image_get_chunk(uint32_t u32L_chunk_addr, uint8_t* u8PL_chunk_buf, uint32_t u32L_chunk_size);
bool image_header_check(const image_hdr_t *objPL_img_hdr);
uint32_t image_get_size(void);

#endif // TEST_IMAGE_H_

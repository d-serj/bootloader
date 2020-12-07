/**
 * @file bootloader.h
 * @brief 
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include "img-header.h"

void image_start(const uint8_t *u8PL_image_data, uint32_t u32L_data_size) __attribute__((noreturn));

#endif // BOOTLOADER_H_

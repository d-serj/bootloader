/**
 * @file bootloader.h
 * @brief 
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>

void image_start(uint32_t u32L_vector_addr) __attribute__((noreturn));

#endif // BOOTLOADER_H_

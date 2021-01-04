/**
 * @file storage.h
 * @brief Functions to work with image storage
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>

void storage_get_chunk(const char *cPL_file_name, uint32_t u32L_chunk_addr, uint8_t *u8PL_chunk_buf, uint32_t u32L_chunk_size);
uint32_t storage_get_file_size(const char *cPL_file_name);

#endif // STORAGE_H_

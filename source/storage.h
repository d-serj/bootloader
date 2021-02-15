/**
 * @file storage.h
 * @brief Functions to work with image storage
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>


/**
 * @brief Initialize storage
 */
void storage_init(void);

/**
 * @brief Deinitialize storage
 */
void storage_deinit(void);

/**
 * @brief Get the file chunk from the storage
 * 
 * @param cPL_file_name .. file name to get chunk of
 * @param u32L_chunk_addr .. position to read the chunk
 * @param u8PL_chunk_buf .. buffer to be filled with data
 * @param u32L_chunk_size .. chunk size to read
 * @return uint32_t number of read bytes
 */
uint32_t storage_get_chunk(const char *cPL_file_name,
                           uint32_t u32L_chunk_addr,
                           uint8_t *u8PL_chunk_buf,
                           uint32_t u32L_chunk_size);

/**
 * @brief Get file size
 * 
 * @param cPL_file_name .. filename to give the size of
 * @return uint32_t size of the file, 0 if file was not found
 */
uint32_t storage_get_file_size(const char *cPL_file_name);

#endif // STORAGE_H_

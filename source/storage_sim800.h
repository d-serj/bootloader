/**
 * @file storage.h
 * @brief Functions to work with image storage
 */

#ifndef STORAGE_SIM800_H_
#define STORAGE_SIM800_H_

#include <stdint.h>

enum StorageError
{
  eStorageOk    = 0,
  eStorageError = 1,
};

/**
 * @brief Initialize storage
 */
void storage_init(void);

/**
 * @brief Deinitialize storage
 */
void storage_deinit(void);

/**
 * @brief Get file size
 * 
 * @param cPL_file_name .. filename to give the size of
 * @param u32L_file_size .. pointer to the variable where file size will be written
 * @return error code from StorageError enumeration
 */
int8_t storage_get_file_size(const char *cPL_file_name, uint32_t *u32L_file_size);

#endif // STORAGE_SIM800_H_

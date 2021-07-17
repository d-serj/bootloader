/**
 * @file storage.h
 * @brief Functions to work with image storage
 */

#ifndef STORAGE_SIM800_H_
#define STORAGE_SIM800_H_

#include <stdint.h>

#include "storage.h"

enum StorageError
{
  eStorageOk    = 0,
  eStorageError = 1,
};

/**
 * @brief Initialize storage
 */
storage_t *storage_sim800_init_static(void);

#endif // STORAGE_SIM800_H_

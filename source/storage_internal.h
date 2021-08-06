/**
 * @file storage_internal.h
 */

#ifndef STORAGE_INTERNAL_H_
#define STORAGE_INTERNAL_H_

#include <stdint.h>

#include "storage.h"

storage_t *storage_internal_init_static(uint32_t u32L_flash_start_addr);

#endif // STORAGE_INTERNAL_H_

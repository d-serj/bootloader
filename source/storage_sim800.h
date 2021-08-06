/**
 * @file storage.h
 * @brief Functions to work with image storage
 */

#ifndef STORAGE_SIM800_H_
#define STORAGE_SIM800_H_

#include <stdint.h>

#include "drivers/usart_driver.h"

#include "storage.h"

enum StorageError
{
  eStorageOk    = 0,
  eStorageError = 1,
};

/**
 * @brief Get SIM800 storage instance
 * 
 * @param objPL_uart .. pointer to UART instance to communicate with SIM800
 * @return storage_t* pointer to storage
 */
storage_t *storage_sim800_init_static(usart_instance_t *objPL_uart);

void storage_sim800_deinit(void);

#endif // STORAGE_SIM800_H_

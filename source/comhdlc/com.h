/**
 * @file com.h
 * @brief Communication module with host PC. Header file
 */

#ifndef COM_H_
#define COM_H_

#include <storage/storage.h>

void com_init(usart_instance_t *objPL_uart);

void com_set_storage_to_write_file(storage_t *objPL_storage);

void com_deinit(void);

void com_run(void);

bool com_is_master_connected(uint16_t u16L_timeout);

void com_systick_clbk(void *PL_user_data);

bool com_file_write_is_finished(void);

#endif // COM_H_

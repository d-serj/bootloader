/**
 * @file sim800.h
 */

#ifndef SIM800_SIM800_H_
#define SIM800_SIM800_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Power on SIM800 module
 * @note  For more information about turn on/off timings see "SIM800_Hardware Design_V1.09" page 22
 */
bool sim800_power_on(void);

/**
 * @brief Power off SIM800 module
 * @note  For more information about turn on/off timings see "SIM800_Hardware Design_V1.09" page 22
 */
bool sim800_power_off(void);

/**
 * @brief Get the size of a given file
 * 
 * @param cPL_file_name .. pointer to the file name
 * @return uint32_t requested file size. 0 if there is no file with such name
 */
uint32_t sim800_storage_get_file_size(const char *cPL_file_name);


#endif /* SIM800_SIM800_H_ */

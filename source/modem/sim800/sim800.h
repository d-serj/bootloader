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

#endif /* SIM800_SIM800_H_ */

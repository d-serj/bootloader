/**
 * @file comhdlc.h
 * @brief 
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Init comhdlc
 */
void comhdlc_init(void);

/**
 * @brief Deinitialize comhdlc
 */
void comhdlc_deinit(void);

bool comhdlc_get_command(uint16_t *u16PL_command, uint32_t u32L_timeout);

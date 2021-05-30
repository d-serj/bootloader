/**
 * @file comhdlc.h
 * @brief 
 */

#include <stdint.h>
#include <stdbool.h>

#define COMHDLC_RESPONSE  (0xBEEF)

enum eComHdlcCommands
{
  eComHdlcResponseError = (1 << 0),
  eComHdlcResponseReady = (1 << 1),
};

/**
 * @brief Init comhdlc
 */
void comhdlc_init(void);

/**
 * @brief Deinitialize comhdlc
 */
void comhdlc_deinit(void);

bool comhdlc_get_data(uint8_t *u8PL_buff, uint16_t u16L_buff_size, uint32_t u32L_timeout);

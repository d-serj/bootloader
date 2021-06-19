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

/**
 * @brief Get data received by comhdlc
 * 
 * @param u8PL_buff .. buffer where received data will be stored
 * @param u16L_buff_size .. buffer size
 * @param u32L_timeout .. timeout to wait for a new message
 * @return true if data was received in u32L_timeout time, false otherwise
 */
bool comhdlc_get_data(uint8_t *u8PL_buff, uint16_t u16L_buff_size, uint32_t u32L_timeout);

/**
 * @brief 
 * 
 * @param u32L_timeout .. timeout to wait for a connection with the host
 * @return true .. if connection was established, false otherwise
 */
bool comhdlc_is_connected(uint32_t u32L_timeout);

/**
 * @file usart.h
 * @brief Usart program module
 */

#ifndef USART_H_
#define USART_H_

#include <stdint.h>

/** RX buffer */
extern uint8_t u8_data[256];

/** Flag that indicates that data was received */
extern uint8_t u8_data_received;

/**
 * @brief Initialize USART
 */
void usart_init(void);

/**
 * @brief Deinitialize USART
 */
void usart_deinit(void);

/**
 * @brief Send null terminated string via USART
 * @param u8PL_str .. pointer to the string
 * @warning String must have null terminator at the end
 */
void usart_send_string(const uint8_t *u8PL_str);

#endif // USART_H_


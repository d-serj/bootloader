/**
 * @file usart.h
 * @brief Usart program module
 */

#ifndef USART_H_
#define USART_H_

#include <stdint.h>

#include <libopencm3/stm32/usart.h>

#include <utilities/ringbuffer.h>

enum USART_INSTANCES
{
  eUART2 = USART2,
  eUART4 = UART4,
};
/** Type definition of UART number */
typedef enum USART_INSTANCES uart_num_t;

struct usart_instance
{
  ring_buffer_t obj_buffer;
  uart_num_t e_instance;
};
typedef struct usart_instance usart_instance_t;

/**
 * @brief Initialize USART
 * @param eL_uart_num .. UART instance number.
 *        Support two instances eUART2 and eUART4
 */
void usart_setup(usart_instance_t *objPL_uart, uart_num_t eL_uart_num);

/**
 * @brief Deinitialize USART
 */
void usart_deinit(usart_instance_t *objPL_uart);

/**
 * @brief Send null terminated string via USART
 * @param u8PL_str .. pointer to the string
 * @warning String must have null terminator at the end
 */
void usart_send_string(const usart_instance_t *objPL_uart, const char *cPL_str);

/**
 * @brief Clear receive buffer
 */
void usart_clear_rx_buf(usart_instance_t *objPL_uart);

/**
 * @brief Get byte from USART stream
 * 
 * @param objPL_uart .. pointer to USART instance
 * @return uint8_t received byte
 */
uint8_t usart_get_byte(usart_instance_t *objPL_uart);

#endif // USART_H_


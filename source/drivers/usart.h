/**
 * @file usart.h
 * @brief Usart program module
 */

#ifndef USART_H_
#define USART_H_

#include <stdint.h>

#include <libopencm3/stm32/usart.h>

#include <ringbuffer.h>

#define UART_RX_BUFFER_SIZE  2412u

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
 */
void usart_setup(usart_instance_t *objPL_uart);

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

#endif // USART_H_


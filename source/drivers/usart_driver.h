/**
 * @file usart_driver.h
 * @brief Usart program module
 */

#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

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
void usart_flush(usart_instance_t *objPL_uart);

/**
 * @brief Get byte from USART stream
 * 
 * @param objPL_uart .. pointer to USART instance
 * @param u8PL_byte .. pointer to uint8_t where received data will be stored
 * @param u8L_timeout .. timeout to wait for a byte
 * @return uint8_t 1 if byte was received, 0 otherwise
 */
uint8_t usart_get_byte(usart_instance_t *objPL_uart, uint8_t *u8PL_byte, uint8_t u8L_timeout);

/**
 * @brief Send raw data via UART
 * 
 * @param objPL_uart .. pointer to USART instance
 * @param u8PL_data .. pointer to the data
 * @param u16L_data_size .. data size
 */
void usart_send_raw(const usart_instance_t *objPL_uart,
  const uint8_t *u8PL_data, uint16_t u16L_data_size);

#endif // USART_DRIVER_H_


/**
 * @file usart_driver.c
 * @brief Usart program module
 */

#include <stdint.h>
#include <stddef.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>

#include <system/assert.h>
#include <utilities/toolbox.h>
#include <utilities/ringbuffer.h>
#include <delay.h>

#include "usart_driver.h"

static ring_buffer_t *objSP_usart2_ring_buff = NULL;
static ring_buffer_t *objSP_uart4_ring_buff  = NULL;

void usart_setup(usart_instance_t *objPL_uart, uart_num_t eL_uart_num)
{
  ASSERT(objPL_uart != NULL);
  ASSERT((eL_uart_num == eUART2) || (eL_uart_num == eUART4));

  objPL_uart->e_instance = eL_uart_num;

  rcc_periph_clock_enable(RCC_AFIO);

  if (eL_uart_num == eUART2)
  {
    rcc_periph_clock_enable(RCC_USART2);

    nvic_enable_irq(NVIC_USART2_IRQ);
    
    /* Setup GPIO pin GPIO_USART2_TX. */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);

    /* Setup GPIO pin GPIO_USART2_RE_RX on GPIO port A for receive. */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX);

    objSP_usart2_ring_buff = &objPL_uart->obj_buffer;
  }
  else if (eL_uart_num == eUART4)
  {
    rcc_periph_clock_enable(RCC_UART4);

    nvic_enable_irq(NVIC_UART4_IRQ);

    /* Setup GPIO pin GPIO_USART2_TX. */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_UART4_TX);

    /* Setup GPIO pin GPIO_USART2_RE_RX on GPIO port A for receive. */
    gpio_set_mode(GPIOC, GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT, GPIO_UART4_RX);

    objSP_uart4_ring_buff = &objPL_uart->obj_buffer;
  }

  /* Setup UART parameters. */
  usart_set_baudrate(eL_uart_num, 9600);
  usart_set_databits(eL_uart_num, 8);
  usart_set_stopbits(eL_uart_num, USART_STOPBITS_1);
  usart_set_mode(eL_uart_num, USART_MODE_TX_RX);
  usart_set_parity(eL_uart_num, USART_PARITY_NONE);
  usart_set_flow_control(eL_uart_num, USART_FLOWCONTROL_NONE);

  /* Enable USART Receive interrupt. */
  usart_enable_rx_interrupt(eL_uart_num);

  usart_disable_tx_interrupt(eL_uart_num);

  ring_buffer_init(&objPL_uart->obj_buffer);

  /* Finally enable the USART. */
  usart_enable(eL_uart_num);
}

void usart_deinit(usart_instance_t *objPL_uart)
{
  const uart_num_t eL_uart_num = objPL_uart->e_instance;
  ASSERT((eL_uart_num == eUART2) || (eL_uart_num == eUART4));

  if (eL_uart_num == eUART2)
  {
    nvic_disable_irq(NVIC_USART2_IRQ);
    rcc_periph_clock_disable(RCC_USART2);
  }
  else if (eL_uart_num == eUART4)
  {
    nvic_disable_irq(NVIC_UART4_IRQ);
    rcc_periph_clock_disable(RCC_UART4);
  }

  rcc_periph_clock_disable(RCC_AFIO);
  usart_disable(eL_uart_num);
}

void usart_send_string(const usart_instance_t *objPL_uart, const char *cPL_str)
{
  ASSERT(objPL_uart != NULL);
  ASSERT((objPL_uart->e_instance == eUART2) || (objPL_uart->e_instance == eUART4));

  while(*cPL_str != '\0')
  {
    usart_send_blocking(objPL_uart->e_instance, (uint16_t)*cPL_str);
    ++cPL_str;
  }
}

void usart_send_raw(const usart_instance_t *objPL_uart,
  const uint8_t *u8PL_data, uint16_t u16L_data_size)
{
  ASSERT(objPL_uart != NULL);
  ASSERT((objPL_uart->e_instance == eUART2) || (objPL_uart->e_instance == eUART4));

  for (uint16_t u16L_i = 0; u16L_i < u16L_data_size; ++u16L_i)
  {
    usart_send_blocking(objPL_uart->e_instance, (uint16_t)u8PL_data[u16L_i]);
  }
}

void usart_flush(usart_instance_t *objPL_uart)
{
  ASSERT(objPL_uart != NULL);
  ASSERT((objPL_uart->e_instance == eUART2) || (objPL_uart->e_instance == eUART4));

  ring_buffer_t *objPL_ring_buff = &objPL_uart->obj_buffer;

  while (ring_buffer_is_empty(objPL_ring_buff) == 0)
  {
    ring_buffer_dequeue(objPL_ring_buff, NULL);
  }
}

uint8_t usart_get_byte_with_timeout(usart_instance_t *objPL_uart, uint8_t *u8PL_byte, uint8_t u8L_timeout)
{
  ASSERT(objPL_uart != NULL);

  uint8_t u8L_delay = 0;
  uint8_t u8L_ret   = 0;

  while (u8L_delay <= u8L_timeout)
  {
    u8L_ret = ring_buffer_dequeue(&objPL_uart->obj_buffer, u8PL_byte);

    if (u8L_ret > 0)
    {
      break;
    }

    ++u8L_delay;
    delay(1);
  }

  return u8L_ret;
}

void usart2_isr(void)
{
  /* Check if we were called because of RXNE. */
  if (((USART2_CR1 & USART_CR1_RXNEIE) != 0)
    && ((USART2_SR & USART_SR_RXNE) != 0))
  {
    ring_buffer_queue(objSP_usart2_ring_buff, (uint8_t)usart_recv(USART2));
  }
}

void uart4_isr(void)
{
  /* Check if we were called because of RXNE. */
  if (((UART4_CR1 & USART_CR1_RXNEIE) != 0)
    && ((UART4_SR & USART_SR_RXNE) != 0))
  {
    const uint8_t u8L_data = (uint8_t)usart_recv(UART4);
    ring_buffer_queue(objSP_uart4_ring_buff, u8L_data);
  }
}

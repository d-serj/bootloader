/**
 * @file usart.c
 * @brief Usart program module
 */

#include <stdint.h>

#include "libopencm3/stm32/usart.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/cm3/nvic.h"

#include "toolbox.h"
#include "usart.h"

#define USART_NUMBER        USART2
#define USART_GPIO_TX(NAME)  GPIO_##NAME##_TX
#define USART_GPIO_RX(NAME)  GPIO_##NAME##_RX
#define USART_IRQ(NAME)      NVIC_##NAME##_IRQ


uint8_t u8_uart_data[256]        = { 0 };
const uint32_t u32_data_arr_size = ARRAY_SIZE(u8_uart_data);
uint8_t u8_uart_data_received    = 0;

void usart_setup(void)
{
  nvic_enable_irq(USART_IRQ(USART2));

  /* Setup GPIO pin GPIO_USART2_TX. */
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, USART_GPIO_TX(USART2));

  /* Setup GPIO pin GPIO_USART2_RE_RX on GPIO port A for receive. */
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_FLOAT, USART_GPIO_RX(USART2));

  /* Setup UART parameters. */
  usart_set_baudrate(USART_NUMBER, 9600);
  usart_set_databits(USART_NUMBER, 8);
  usart_set_stopbits(USART_NUMBER, USART_STOPBITS_1);
  usart_set_mode(USART_NUMBER, USART_MODE_TX_RX);
  usart_set_parity(USART_NUMBER, USART_PARITY_NONE);
  usart_set_flow_control(USART_NUMBER, USART_FLOWCONTROL_NONE);

  /* Enable USART Idle line detection */
  USART_CR1(USART_NUMBER) |= USART_CR1_IDLEIE;
  /* Enable USART Receive interrupt. */
  USART_CR1(USART_NUMBER) |= USART_CR1_RXNEIE;

  /* Finally enable the USART. */
  usart_enable(USART_NUMBER);
}

void usart_deinit(void)
{
  nvic_disable_irq(USART_IRQ(USART2));
  usart_disable(USART_NUMBER);
}

void usart_send_string(const uint8_t *u8PL_str)
{
  while(*u8PL_str != '\0')
  {
    usart_send_blocking(USART_NUMBER, (uint16_t)*u8PL_str);
    ++u8PL_str;
  }
}

void usart_clear_rx_buf(void)
{
  for (uint16_t u16L_i = 0; u16L_i < ARRAY_SIZE(u8_uart_data); ++u16L_i)
  {
    u8_uart_data[u16L_i] = 0;
  }
}

#if (USART_NUMBER == USART2)
void usart2_isr(void)
{
  static uint16_t u16SL_idx  = 0;

  /* Check if we were called because of RXNE. */
  if (((USART_CR1(USART_NUMBER) & USART_CR1_RXNEIE) != 0)
    && ((USART_SR(USART_NUMBER) & USART_SR_RXNE) != 0))
  {
    u8_uart_data[u16SL_idx] = usart_recv(USART_NUMBER);

    /* Retrieve the data from the peripheral. */
    if (u16SL_idx < u32_data_arr_size)
    {
      ++u16SL_idx;
    }
    else
    {
      u16SL_idx = 0;
    }
  }
  else if (((USART_CR1(USART_NUMBER) & USART_CR1_IDLEIE) != 0)
    && ((USART_SR(USART_NUMBER) & USART_SR_IDLE) != 0))
  {
    volatile uint32_t u32L_tmp = 0;

    // Clear IDLE flag by reading status register first
    // And follow by reading data register
    u32L_tmp = USART_SR(USART_NUMBER);
    u32L_tmp = USART_DR(USART_NUMBER);

    u8_uart_data[u16SL_idx] = '\0';

    u8_uart_data_received = 1;
    u16SL_idx        = 0;
  }
}
#endif

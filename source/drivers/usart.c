/**
 * @file usart.c
 * @brief Usart program module
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/usart.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/cm3/nvic.h"

#include "toolbox.h"
#include "usart.h"


/** USART2 RX buffer */
static uint8_t u8_usart2_data[UART_RX_BUFFER_SIZE];

/** USART2 RX buffer */
static uint8_t u8_uart4_data[UART_RX_BUFFER_SIZE];

/** RX buffer size */
const uint16_t u16_usart2_data_arr_size = ARRAY_SIZE(u8_usart2_data);
const uint16_t u16_uart4_data_arr_size  = ARRAY_SIZE(u8_uart4_data);

/** Flag that indicates that data was received */
volatile uint8_t u8_usart2_data_received;
volatile uint8_t u8_uart4_data_received;


void usart_setup(usart_instance_t *objPL_uart)
{
  assert(objPL_uart != NULL);
  const uart_num_t eL_uart_num = objPL_uart->e_instance;
  assert((eL_uart_num == eUART2) || (eL_uart_num == eUART4));

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

    objPL_uart->u8P_buffer        = u8_usart2_data;
    objPL_uart->u16_buff_size     = u16_usart2_data_arr_size;
    objPL_uart->u8P_data_received = &u8_usart2_data_received;
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

    objPL_uart->u8P_buffer        = u8_uart4_data;
    objPL_uart->u16_buff_size     = u16_uart4_data_arr_size;
    objPL_uart->u8P_data_received = &u8_uart4_data_received;
  }

  /* Setup UART parameters. */
  usart_set_baudrate(eL_uart_num, 9600);
  usart_set_databits(eL_uart_num, 8);
  usart_set_stopbits(eL_uart_num, USART_STOPBITS_1);
  usart_set_mode(eL_uart_num, USART_MODE_TX_RX);
  usart_set_parity(eL_uart_num, USART_PARITY_NONE);
  usart_set_flow_control(eL_uart_num, USART_FLOWCONTROL_NONE);

  /* Enable USART Idle line detection */
  USART_CR1(eL_uart_num) |= USART_CR1_IDLEIE;
  /* Enable USART Receive interrupt. */
  USART_CR1(eL_uart_num) |= USART_CR1_RXNEIE;

  /* Finally enable the USART. */
  usart_enable(eL_uart_num);
}

void usart_deinit(usart_instance_t *objPL_uart)
{
  const uart_num_t eL_uart_num = objPL_uart->e_instance;
  assert((eL_uart_num == eUART2) || (eL_uart_num == eUART4));

  if (eL_uart_num == eUART2)
  {
    nvic_disable_irq(NVIC_USART2_IRQ);
  }
  else if (eL_uart_num == eUART4)
  {
    nvic_disable_irq(NVIC_UART4_IRQ);
  }

  usart_disable(eL_uart_num);
}

void usart_send_string(const usart_instance_t *objPL_uart, const char *cPL_str)
{
  assert(objPL_uart != NULL);
  assert((objPL_uart->e_instance == eUART2) || (objPL_uart->e_instance == eUART4));

  while(*cPL_str != '\0')
  {
    usart_send_blocking(objPL_uart->e_instance, (uint16_t)*cPL_str);
    ++cPL_str;
  }
}

void usart_send_raw(const usart_instance_t *objPL_uart,
  const uint8_t *u8PL_data, uint16_t u16L_data_size)
{
  assert(objPL_uart != NULL);
  assert((objPL_uart->e_instance == eUART2) || (objPL_uart->e_instance == eUART4));

  for (uint16_t u16L_i = 0; u16L_i < u16L_data_size; ++u16L_i)
  {
    usart_send_blocking(objPL_uart->e_instance, (uint16_t)u8PL_data[u16L_i]);
  }
}

void usart_clear_rx_buf(usart_instance_t *objPL_uart)
{
  assert(objPL_uart != NULL);
  assert((objPL_uart->e_instance == eUART2) || (objPL_uart->e_instance == eUART4));

  for (uint16_t u16L_i = 0; u16L_i < objPL_uart->u16_buff_size; ++u16L_i)
  {
    objPL_uart->u8P_buffer[u16L_i] = 0;
  }
}

void usart2_isr(void)
{
  static uint16_t u16SL_idx  = 0;

  /* Check if we were called because of RXNE. */
  if (((USART2_CR1 & USART_CR1_RXNEIE) != 0)
    && ((USART2_SR & USART_SR_RXNE) != 0))
  {
    u8_usart2_data[u16SL_idx] = usart_recv(USART2);

    /* Retrieve the data from the peripheral. */
    if (u16SL_idx < u16_usart2_data_arr_size)
    {
      ++u16SL_idx;
    }
    else
    {
      u16SL_idx = 0;
    }
  }
  else if (((USART2_CR1 & USART_CR1_IDLEIE) != 0)
    && ((USART2_SR & USART_SR_IDLE) != 0))
  {
    volatile uint32_t u32L_tmp = 0;

    // Clear IDLE flag by reading status register first
    // And follow by reading data register
    u32L_tmp = USART2_SR;
    u32L_tmp = USART2_DR;

    u8_usart2_data[u16SL_idx] = '\0';

    u8_usart2_data_received = 1;
    u16SL_idx               = 0;
  }
}

void uart4_isr(void)
{
  static uint16_t u16SL_idx  = 0;

  /* Check if we were called because of RXNE. */
  if (((UART4_CR1 & USART_CR1_RXNEIE) != 0)
    && ((UART4_SR & USART_SR_RXNE) != 0))
  {
    u8_uart4_data[u16SL_idx] = usart_recv(UART4);

    /* Retrieve the data from the peripheral. */
    if (u16SL_idx < u16_uart4_data_arr_size)
    {
      ++u16SL_idx;
    }
    else
    {
      u16SL_idx = 0;
    }
  }
  else if (((UART4_CR1 & USART_CR1_IDLEIE) != 0)
    && ((UART4_SR & USART_SR_IDLE) != 0))
  {
    volatile uint32_t u32L_tmp = 0;

    // Clear IDLE flag by reading status register first
    // And follow by reading data register
    u32L_tmp = UART4_SR;
    u32L_tmp = UART4_DR;

    u8_uart4_data[u16SL_idx] = '\0';

    u8_uart4_data_received = 1;
    u16SL_idx              = 0;
  }
}

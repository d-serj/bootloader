/**
 * @file main.c
 * @brief Main file of the program
 */

#include <stdint.h>
#include <stdio.h>

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"

#include "img-header.h"
#include "drivers/usart.h"
#include "drivers/spi.h"
#include "delay.h"
#include "pins.h"
#include "sim800/sim800.h"

static void clock_setup(void)
{
  rcc_clock_setup_in_hsi_out_64mhz();

  /* Enable GPIOC clock. */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable GPIOB clock. */
  rcc_periph_clock_enable(RCC_GPIOB);

  /* Enable GPIOA clock. */
  //rcc_periph_clock_enable(RCC_GPIOA);
  //rcc_periph_clock_enable(RCC_AFIO);

  rcc_periph_clock_enable(RCC_USART2);
  //rcc_periph_clock_enable(RCC_SPI1);
}

static void gpio_setup(void)
{
  gpio_set_mode(CPU_STATUS_GPIO_Port, GPIO_MODE_OUTPUT_2_MHZ,
    GPIO_CNF_OUTPUT_PUSHPULL, CPU_STATUS_Pin);
  gpio_set_mode(SIM800_PWR_OFF_GPIO_Port, GPIO_MODE_OUTPUT_2_MHZ,
    GPIO_CNF_OUTPUT_PUSHPULL, SIM800_PWR_OFF_Pin);
  gpio_set_mode(SIM800_PWR_KEY_GPIO_Port, GPIO_MODE_OUTPUT_2_MHZ,
    GPIO_CNF_OUTPUT_PUSHPULL, SIM800_PWR_KEY_Pin);
  gpio_set_mode(GSM_STATUS_GPIO_Port, GPIO_MODE_INPUT,
    GPIO_CNF_INPUT_FLOAT, GSM_STATUS_Pin);
}

int main(void)
{
  clock_setup();
  systick_setup();
  gpio_setup();
  usart_setup();

  sim800_power_on();
  delay(10000);
  sim800_power_off();
  
  for(;;)
  {
    gpio_set(GPIOC, GPIO0);
    //usart_send_string(u8_data);

    delay(1000);
    gpio_clear(GPIOC, GPIO0);
    delay(1000);
  }

  return 0;
}

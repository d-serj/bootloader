/**
 * @file main.c
 * @brief Main file of the program
 */

#include <stdint.h>
#include <stdio.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/vector.h>

#include "img-header.h"
#include "delay.h"
#include "drivers/usart_driver.h"

#include "pins.h"

static usart_instance_t objS_usart2;
static usart_instance_t objS_usart4;

static void clock_setup(void)
{
  rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    /* Enable GPIOC clock. */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable GPIOB clock. */
  rcc_periph_clock_enable(RCC_GPIOB);

  /* Enable GPIOA clock. */
  rcc_periph_clock_enable(RCC_GPIOA);

  rcc_periph_clock_enable(RCC_AFIO);
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

bool sim800_power_on(void)
{
  if (gpio_get(GSM_STATUS_GPIO_Port, GSM_STATUS_Pin))
  {
    return true;
  }

  gpio_set(SIM800_PWR_OFF_GPIO_Port, SIM800_PWR_OFF_Pin);
  delay(1100);
  gpio_set(SIM800_PWR_KEY_GPIO_Port, SIM800_PWR_KEY_Pin);
  delay(1800);
  gpio_clear(SIM800_PWR_KEY_GPIO_Port, SIM800_PWR_KEY_Pin);

  // Wait till SIM800 will be activated
  delay(3000);

  // Check SIM800 status pin
  return !!gpio_get(GSM_STATUS_GPIO_Port, GSM_STATUS_Pin);
}

int main(void)
{
  clock_setup();
  systick_setup();
  gpio_setup();

  usart_setup(&objS_usart2, eUART2);
  usart_setup(&objS_usart4, eUART4);

  sim800_power_on();
 
  uint8_t u8L_byte = 0;

  for(;;)
  {
    if (usart_get_byte(&objS_usart4, &u8L_byte, 10))
    {
      usart_send_raw(&objS_usart2, &u8L_byte, 1);

      while (usart_get_byte(&objS_usart4, &u8L_byte, 10))
      {
        usart_send_raw(&objS_usart2, &u8L_byte, 1);
      }
    }

    if (usart_get_byte(&objS_usart2, &u8L_byte, 10))
    {
      usart_send_raw(&objS_usart4, &u8L_byte, 1);

      while (usart_get_byte(&objS_usart2, &u8L_byte, 10))
      {
        usart_send_raw(&objS_usart4, &u8L_byte, 1);
      }
    }

    gpio_toggle(CPU_STATUS_GPIO_Port, CPU_STATUS_Pin);
  }

  return 0;
}

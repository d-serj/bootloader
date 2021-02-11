/**
 * @file main.c
 * @brief Main file of the program
 */

#include <stdint.h>
#include <stdio.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/vector.h>

#include "bootloader.h"
#include "img-header.h"
#include "drivers/usart.h"
#include "delay.h"
#include "pins.h"
#include "modem/sim800/sim800.h"
#include "image.h"

static usart_instance_t objS_uart4;

static void clock_setup(void)
{
  rcc_clock_setup_in_hsi_out_24mhz();

  /* Enable GPIOC clock. */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable GPIOB clock. */
  rcc_periph_clock_enable(RCC_GPIOB);

  /* Enable GPIOA clock. */
  rcc_periph_clock_enable(RCC_GPIOA);
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

  usart_setup(&objS_uart4, eUART4);
/*
  while (*objS_uart4.u8P_data_received == 0)
  {
    gpio_toggle(CPU_STATUS_GPIO_Port, CPU_STATUS_Pin);
    delay(100);
  }
  */

  usart_deinit(&objS_uart4);
  systick_deinit();
  rcc_periph_clock_disable(RCC_GPIOC);
  rcc_periph_clock_disable(RCC_GPIOB);
  rcc_periph_clock_disable(RCC_GPIOA);
  rcc_osc_off(RCC_HSI);

  // 1. Check flags
  // 2. Load app or go to update
  // 3. Get image header
  // 4. Image validate header
  // 5. Check CRC32 for the whole file
  // 6. Download image from the beginning to flash it
  // 7. Image flash
  // 8. Check CRC32
  // 6. Image start
  //image_start(objS_uart4.u8P_buffer, *objS_uart4.u16P_rec_bytes);
  const image_hdr_t objL_hdr = image_header_get();
  image_validate(&objL_hdr);
  
  for(;;);

  return 0;
}

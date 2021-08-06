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
#include "drivers/usart_driver.h"
#include "delay.h"
#include "pins.h"
#include "modem/sim800/sim800.h"
#include "utilities/toolbox.h"
#include "drivers/flash.h"
#include "storage_sim800.h"
#include "memory_map.h"
#include "image.h"
#include "com.h"

#include "storage.h"
#include "storage_internal.h"

static usart_instance_t objS_uart2;
static usart_instance_t objS_uart4;

static void clock_setup(void)
{
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_64MHZ]);

  /* Enable GPIOC clock. */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable GPIOB clock. */
  rcc_periph_clock_enable(RCC_GPIOB);

  /* Enable GPIOA clock. */
  rcc_periph_clock_enable(RCC_GPIOA);

  rcc_periph_clock_enable(RCC_AFIO);
}

static void clock_deinit(void)
{
  // rcc_periph_reset_pulse(RST_GPIOC);
  // rcc_periph_reset_pulse(RST_GPIOB);
  // rcc_periph_reset_pulse(RST_GPIOA);
  // rcc_periph_clock_disable(RCC_GPIOC);
  // rcc_periph_clock_disable(RCC_GPIOB);
  // rcc_periph_clock_disable(RCC_GPIOA);
  // rcc_periph_reset_pulse(RST_AFIO);
  // rcc_periph_clock_disable(RCC_AFIO);
  // rcc_peripheral_disable_clock(&RCC_APB1ENR, RCC_APB1ENR_UART4EN | RCC_APB1ENR_USART2EN);
  // rcc_peripheral_disable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN);
  // rcc_osc_off(RCC_HSI);
  // rcc_osc_off(RCC_PLL);

  RCC_AHBENR &= ~(RCC_AHBENR_CRCEN);
	RCC_AHBENR &= ~(RCC_AHBENR_CRCEN);
	RCC_APB2RSTR = 0;
	RCC_APB1RSTR = 0;
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
  systick_init(com_systick_clbk, NULL);
  gpio_setup();

  usart_setup(&objS_uart2, eUART2);
  usart_setup(&objS_uart4, eUART4);

  storage_t *objPL_storage_sim800 = storage_sim800_init_static(&objS_uart2);
  com_init(objPL_storage_sim800, &objS_uart4);

  if (com_is_master_connected(1000))
  {
    while (com_file_write_is_finished() == false)
      ;
  }

  bool image_could_start = false;
  image_t objL_image = { 0 };
  if (image_open(&objL_image, objPL_storage_sim800, "firmware.bin") == 0)
  {
    if (image_validate(&objL_image))
    {
      const int8_t s8L_ret = image_copy(&objL_image, storage_internal_init_static(0x08004000));
      image_could_start = (s8L_ret == 0);
    }
    else
    {
      image_close(&objL_image);
    }
  }

  // 1. Check flags
  // 2. Load app or go to update
  // 3. Get image header
  // 4. Image validate header
  // 5. Check CRC32 for the whole file
  // 6. Download image from the beginning to flash it
  // 7. Image flash
  // 8. Check CRC32
  // 6. Image start

  com_deinit();
  storage_sim800_deinit();

  systick_deinit();
  usart_deinit(&objS_uart2);
  usart_deinit(&objS_uart4);
  clock_deinit();
  
  if (image_could_start)
  {
    image_start(objL_image.obj_img_hdr.u32_vector_addr);
  }

  for(;;)
    ;

  return 0;
}

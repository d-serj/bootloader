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

static void clock_setup(void)
{
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_24MHZ]);

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
  systick_init(com_systick_clbk, NULL);
  gpio_setup();
  com_init();

  for (;;)
  {
    com_run();

    if (com_is_master_connected(1000))
    {
      com_file_write_is_finished();
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

/*
  image_t objL_image = { 0 };
  storage_t *objPL_sim800_stor = storage_sim800_init_static();

  image_open(&objL_image, objPL_sim800_stor, "firmware.bin");

  if (image_validate(&objL_image))
  {
    image_copy(&objL_image, objPL_sim800_stor, storage_internal_init_static());
  }
  */

  com_deinit();
  systick_deinit();
  rcc_periph_clock_disable(RCC_GPIOC);
  rcc_periph_clock_disable(RCC_GPIOB);
  rcc_periph_clock_disable(RCC_GPIOA);
  rcc_osc_off(RCC_HSI);
  
  for(;;);

  return 0;
}

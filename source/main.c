/**
 * @file main.c
 * @brief Main file of the program
 */

#include <stdint.h>
#include <stdio.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/f1/bkp.h>

#include "bootloader.h"
#include "img-header.h"
#include "drivers/usart_driver.h"
#include "delay.h"
#include "pins.h"
#include "modem/sim800/sim800.h"
#include "utilities/toolbox.h"
#include "drivers/flash.h"
#include "drivers/bkp.h"
#include "storage_sim800.h"
#include "memory_map.h"
#include "image.h"
#include "comhdlc/com.h"

#include "storage.h"
#include "storage_internal.h"

static usart_instance_t objS_uart2;
static usart_instance_t objS_uart4;

static void clock_setup(void)
{
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_48MHZ]);

  /* Enable GPIOC clock. */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable GPIOB clock. */
  rcc_periph_clock_enable(RCC_GPIOB);

  /* Enable GPIOA clock. */
  rcc_periph_clock_enable(RCC_GPIOA);

  rcc_periph_clock_enable(RCC_AFIO);

  bkp_init();
}

static void clock_deinit(void)
{
  bkp_deinit();

  rcc_peripheral_disable_clock(&RCC_APB1ENR, RCC_APB1ENR_UART4EN | RCC_APB1ENR_USART2EN);
  rcc_peripheral_disable_clock(&RCC_APB2ENR, RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN);
  RCC_APB1ENR = 0x00000000;
  RCC_APB2ENR = 0x00000000;

  /* Reset the RCC_CFGR register */
  RCC_CFGR = 0x00000000;

  /* Reset the CIR register */
  RCC_CIR = 0x00000000;

  rcc_osc_on(RCC_HSI);
  rcc_wait_for_osc_ready(RCC_HSI);

  rcc_osc_off(RCC_PLL);
  rcc_css_disable();
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
  bool image_could_start = false;
  const uint32_t u32L_main_app_addr = (uint32_t)&__app_start__;

  clock_setup();
  systick_init(com_systick_clbk, NULL);
  gpio_setup();

  usart_setup(&objS_uart2, eUART2);
  usart_setup(&objS_uart4, eUART4);

  // 1. Check flags
  const uint16_t u16L_main_app_cmd = bkp_read(BKP_DR1);

  // 2. Try to connect to the host PC. Wait 10 seconds for a connection
  com_init(&objS_uart4);
  if (com_is_master_connected(1000))
  {
    storage_t *const objPL_storage_sim800 = storage_sim800_init_static(&objS_uart2);
    com_set_storage_to_write_file(objPL_storage_sim800);

    while (com_file_write_is_finished() == false)
      ;
  }

  // 3. Load app or go to update
  image_t objL_image = { 0 };

  storage_t *const objPL_storage_sim800 = storage_sim800_init_static(&objS_uart2);
  if (image_open(&objL_image, objPL_storage_sim800, IMAGE_NAME) == 0)
  {
    if (image_validate(&objL_image))
    {
      const int8_t s8L_ret = image_copy(&objL_image, storage_internal_init_static(u32L_main_app_addr));
      image_could_start    = (s8L_ret == 0);
    }
    else
    {
      image_close(&objL_image);
    }
  }

  // Deinit all the stuff
  com_deinit();
  storage_sim800_deinit();
  delay(2000);

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

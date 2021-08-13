/**
 * @file system.c
 * @brief MCU initialization
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <pins.h>
#include <drivers/bkp.h>

#include "system.h"

static void clock_setup(void);
static void clock_deinit(void);
static void gpio_setup(void);

void system_init(void)
{
  clock_setup();
  gpio_setup();
}

void system_deinit(void)
{
  clock_deinit();
}

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

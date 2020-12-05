/**
 * @file main.c
 * @brief Main file of the program
 */

#include <stdint.h>
#include <stdio.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>
#include <libopencmsis/core_cm3.h>

#include "img-header.h"
#include "drivers/usart.h"
#include "drivers/flash.h"
#include "delay.h"
#include "pins.h"
#include "modem/sim800/sim800.h"

#include "../STM32F1_FLASH.h"

static usart_instance_t objS_usart2 = { .e_instance = eUART2 };
static usart_instance_t objS_uart4  = { .e_instance = eUART4 };

static void clock_setup(void)
{
  rcc_clock_setup_in_hsi_out_64mhz();

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

static void start_app(void *pc, void *sp) __attribute__((naked));

static void start_app(void *pc, void *sp)
{
  __asm
  ("           \n\
        msr msp, r1 /* load r1 into MSP */\n\
        bx r0       /* branch to the address at r0 */\n\
  ");
}

int main(void)
{
  clock_setup();
  systick_setup();
  gpio_setup();
  usart_setup(&objS_usart2);
  usart_setup(&objS_uart4);

  while (*objS_uart4.u8P_data_received == 0)
  {
    gpio_toggle(CPU_STATUS_GPIO_Port, CPU_STATUS_Pin);
    delay(100);
  }

  image_hdr_t* objPL_img = (image_hdr_t*)objS_uart4.u8P_buffer;
  
  flash_program_data(0x8004000, objS_uart4.u8P_buffer, *objS_uart4.u16P_rec_bytes);

  const vector_table_t *objPL_vector =
    (const vector_table_t *)objPL_img->u32_vector_addr;

  usart_deinit(&objS_usart2);
  usart_deinit(&objS_uart4);
  systick_deinit();
  rcc_periph_clock_disable(RCC_GPIOC);
  rcc_periph_clock_disable(RCC_GPIOB);
  rcc_periph_clock_disable(RCC_GPIOA);

  // Disable
  __disable_irq();
  SCB_VTOR = (uint32_t)objPL_vector;
  __enable_irq(); 

  start_app(objPL_vector->reset, objPL_vector->initial_sp_value);


  //sim800_power_on();
  //delay(2000);
  //sim800_power_off();
  
  for(;;);

  return 0;
}

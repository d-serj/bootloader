/**
 * @file bootloader.c
 * @brief 
 */

#include <stdint.h>

#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>
#include <libopencmsis/core_cm3.h>

#include <libopencm3/stm32/gpio.h>

#include "pins.h"
#include "img-header.h"
#include "delay.h"
#include "drivers/usart.h"
#include "drivers/flash.h"
#include "memory_map.h"

#include "bootloader.h"

static inline void __set_MSP(volatile uint32_t topOfMainStack)
{
  //  Set the stack pointer.
  asm("msr msp, %0" : : "r" (topOfMainStack));
}

void image_start(const uint8_t *u8PL_image_data, uint32_t u32L_data_size)
{
  flash_program_data((uint32_t)&__app_start__, u8PL_image_data, u32L_data_size);

  const image_hdr_t *objPL_img = (const image_hdr_t*)u8PL_image_data;
  const vector_table_t *objPL_vector =
    (const vector_table_t *)objPL_img->u32_vector_addr;

  // Disable interrupts before modifing VTOR
  __disable_irq();
  // Ensure all memory operations are finished before modifing VTOR
  asm("dmb");
  SCB_VTOR = (uint32_t)objPL_vector & 0xFFFFFFF8;
  asm("dsb");
  __enable_irq();

  __set_MSP((volatile uint32_t)objPL_vector->initial_sp_value);
  // Call the reset handler
  // (the construct below is 'pointer to a pointer to a function that takes no arguments and returns void')
  (*(void (**)())(0x8004200 + 4))();
  __builtin_unreachable();
}

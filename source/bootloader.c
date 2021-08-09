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

#include "memory_map.h"
#include "bootloader.h"

static inline __attribute__((always_inline)) void __set_MSP(volatile uint32_t topOfMainStack)
{
  //  Set the stack pointer.
  asm volatile("msr msp, %0" : : "r" (topOfMainStack) : );
}

void image_start(uint32_t u32L_vector_addr)
{
  const vector_table_t *objPL_vector =
    (const vector_table_t *)u32L_vector_addr;

  // Disable interrupts before modifing VTOR
  __disable_irq();
  // Ensure all memory operations are finished before modifing VTOR
  asm("dmb");
  SCB_VTOR = (uint32_t)objPL_vector & 0xFFFFF;
  asm("dsb");
  __enable_irq();

  __set_MSP((volatile uint32_t)objPL_vector->initial_sp_value);
  // Call the reset handler
  // (the construct below is 'pointer to a pointer to a function that takes no arguments and returns void')
  void (*reset_handler)(void) = (void *)*(volatile uint32_t *)(u32L_vector_addr + 4);
  reset_handler();
  __builtin_unreachable();
}

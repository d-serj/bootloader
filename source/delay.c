/**
 * @file delay.c
 */

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/cm3/systick.h"

#include "delay.h"

// Storage for our monotonic system clock.
// Note that it needs to be volatile since we're modifying it from an interrupt.
static volatile uint64_t u64S_millis = 0;

void systick_setup(void)
{
  // Set the systick clock source to our main clock
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  // Clear the Current Value Register so that we start at 0
  STK_CVR = 0;
  // In order to trigger an interrupt every millisecond, we can set the reload
  // value to be the speed of the processor / 1000 - 1
  systick_set_reload(rcc_ahb_frequency / 1000 - 1);
  // Enable interrupts from the system tick clock
  systick_interrupt_enable();
  // Enable the system tick counter
  systick_counter_enable();
}

uint64_t millis(void)
{
  return u64S_millis;
}

void delay(uint64_t u64L_duration)
{
  const uint64_t u64L_until = millis() + u64L_duration;
  while (millis() < u64L_until)
    ;
}

/**
 * @brief Interrupt handler for the systick reload interrupt.
 */
void sys_tick_handler(void)
{
  // Increment our monotonic clock
  u64S_millis++;
}

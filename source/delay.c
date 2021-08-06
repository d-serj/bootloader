/**
 * @file delay.c
 */

#include <stddef.h>

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/cm3/systick.h"

#include "delay.h"

static volatile uint64_t u64S_millis = 0;
static systick_clbk_t objPS_callback = NULL;
static void *PS_user_data            = NULL;

void systick_init(systick_clbk_t objPL_callback, void *PL_user_data)
{
  // Set the systick clock source to our main clock
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_clear();
  // In order to trigger an interrupt every millisecond, we can set the reload
  // value to be the speed of the processor / 1000 - 1
  systick_set_reload(rcc_ahb_frequency / 1000 - 1);
  // Enable interrupts from the system tick clock
  systick_interrupt_enable();
  // Enable the system tick counter
  systick_counter_enable();

  objPS_callback = objPL_callback;
  PS_user_data   = PL_user_data;
}

void systick_deinit(void)
{
  systick_clear();
  systick_interrupt_disable();
  systick_counter_disable();
}

uint64_t millis(void)
{
  return u64S_millis;
}

void delay(uint64_t u64L_duration)
{
  const uint64_t u64L_until = millis() + u64L_duration;
  while (millis() < u64L_until)
  {
    __asm__("nop");
  };
}

/**
 * @brief Interrupt handler for the systick reload interrupt.
 */
void sys_tick_handler(void)
{
  if (objPS_callback)
  {
    objPS_callback(PS_user_data);
  }

  // Increment our monotonic clock
  ++u64S_millis;
}

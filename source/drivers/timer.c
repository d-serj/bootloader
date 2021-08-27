/**
 * @file timer.c
 */

#include <stddef.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "timer.h"


#include <libopencm3/stm32/gpio.h>
#include <pins.h>
#include <utilities/toolbox.h>

static timer_callback_t objPS_callback = NULL;
static void *PS_user_context           = NULL;

void timer_init(void)
{
  /* Enable TIM clock. */
  rcc_periph_clock_enable(RCC_TIM6);
  /* Enable TIM interrupt. */
  nvic_enable_irq(NVIC_TIM6_IRQ);
  /* Reset TIM peripheral to defaults. */
  rcc_periph_reset_pulse(RST_TIM6);

  timer_set_prescaler(TIM6, (rcc_ahb_frequency / 1000U - 1U));
  // Once a 100 millisecond
  timer_set_period(TIM6, 100U);

  timer_enable_irq(TIM6, TIM_DIER_UIE);
}

void timer_start(timer_callback_t objPL_callback, void *PL_user_context)
{
  objPS_callback  = objPL_callback;
  PS_user_context = PL_user_context;

  /* Counter enable. */
  timer_set_counter(TIM6, 0U);
  timer_enable_counter(TIM6);
}

void timer_stop(void)
{
  objPS_callback  = NULL;
  PS_user_context = NULL;

  timer_disable_counter(TIM6);
}

void timer_deinit(void)
{
  timer_disable_irq(TIM6, TIM_DIER_UIE);
  timer_disable_counter(TIM6);

  rcc_periph_reset_pulse(RST_TIM6);
  rcc_periph_clock_disable(RCC_TIM6);

  nvic_disable_irq(NVIC_TIM6_IRQ);
}

void timer_change_period(uint16_t u16L_new_period)
{
  timer_set_period(TIM6, u16L_new_period);
}

void tim6_isr(void)
{
  if (timer_get_flag(TIM6, TIM_SR_UIF))
  {
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM6, TIM_SR_UIF);

    if (objPS_callback)
    {
      objPS_callback(PS_user_context);
    }
  }
}

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

void timer_init(timer_callback_t objPL_callback, void *PL_user_context)
{
  objPS_callback  = objPL_callback;
  PS_user_context = PL_user_context;

  /* Enable TIM2 clock. */
  rcc_periph_clock_enable(RCC_TIM2);

  /* Enable TIM2 interrupt. */
  nvic_enable_irq(NVIC_TIM2_IRQ);

  /* Reset TIM2 peripheral to defaults. */
  rcc_periph_reset_pulse(RST_TIM2);

  /* Timer global mode:
   * - No divider
   * - Alignment edge
   * - Direction up
   */
  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
    TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  /* Sets the prescaler to have the timer run at 5kHz */
  timer_set_prescaler(TIM2, (rcc_ahb_frequency / 1000 - 1));

  /* Disable preload. */
  timer_disable_preload(TIM2);
  timer_continuous_mode(TIM2);

  /* count full range, as we'll update compare value continuously */
  timer_set_period(TIM2, 65535);

  /* Set the initual output compare value for OC1. */
  timer_set_oc_value(TIM2, TIM_OC1, 500);

  /* Counter enable. */
  timer_enable_counter(TIM2);

  /* Enable Channel 1 compare interrupt to recalculate compare values */
  timer_enable_irq(TIM2, TIM_DIER_CC1IE);
}

void timer_deinit(void)
{
  timer_disable_irq(TIM2, TIM_DIER_CC1IE);
  timer_disable_counter(TIM2);

  rcc_periph_reset_pulse(RST_TIM2);
  rcc_periph_clock_disable(RCC_TIM2);

  nvic_disable_irq(NVIC_TIM2_IRQ);
}

void timer_change_period(uint16_t u16L_new_period)
{
  const uint16_t u16L_compare_time = timer_get_counter(TIM2);
  const uint16_t u16L_frequency    = u16L_new_period;
	const uint16_t u16L_new_time     = u16L_compare_time + u16L_frequency;

  timer_set_oc_value(TIM2, TIM_OC1, u16L_new_time);
}

void tim2_isr(void)
{
  if (timer_get_flag(TIM2, TIM_SR_CC1IF))
  {
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM2, TIM_SR_CC1IF);

    if (objPS_callback)
    {
      objPS_callback(PS_user_context);
    }
  }
}

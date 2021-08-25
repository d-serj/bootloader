/**
 * @file led.c
 * @brief LED control driver
 */

#include <stdint.h>
#include <stddef.h>

#include <libopencm3/stm32/gpio.h>

#include <pins.h>
#include <utilities/toolbox.h>

#include "led.h"
#include "timer.h"

#define LED_TIME           (100U)
#define LED_PAUSE_500MS    (500U)
#define LED_PAUSE_1S       (LED_PAUSE_500MS * 2U)

typedef struct
{
  const uint16_t *u16P_freq_arr;
  uint16_t u16_freq_arr_size;
  uint16_t u16_idx;
} led_animation_t;

static void led_timer_callback(void *PL_ctx);

static const uint16_t u16PS_start_freq[] =
{
  LED_TIME,
  LED_PAUSE_500MS,
  LED_TIME,
  LED_PAUSE_500MS,
  LED_TIME,
};

static const uint16_t u16PS_error_freq[] =
{
  LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_PAUSE_1S,
  LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_PAUSE_1S,
  LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_TIME,
};

static led_animation_t objS_led_anim = {0};

void led_cpu_init(void)
{
  timer_init();
}

void led_cpu_deinit(void)
{
  timer_deinit();
}

void led_cpu_indicate_start(void)
{
  objS_led_anim.u16P_freq_arr     = u16PS_start_freq;
  objS_led_anim.u16_freq_arr_size = ARRAY_SIZE(u16PS_start_freq);
  objS_led_anim.u16_idx           = 0;
  timer_start(led_timer_callback, (void *)&objS_led_anim);
}

void led_cpu_indicate_error(void)
{
  objS_led_anim.u16P_freq_arr     = u16PS_error_freq;
  objS_led_anim.u16_freq_arr_size = ARRAY_SIZE(u16PS_error_freq);
  objS_led_anim.u16_idx           = 0;
  timer_start(led_timer_callback, (void *)&objS_led_anim);
}

static void led_timer_callback(void *PL_ctx)
{
  led_animation_t * const objPL_animation = (led_animation_t *)PL_ctx;

  if (objPL_animation->u16_idx == objPL_animation->u16_freq_arr_size)
  {
    objPL_animation->u16_idx = 0;
    gpio_clear(CPU_STATUS_GPIO_Port, CPU_STATUS_Pin);
    timer_stop();
    return;
  }

  timer_change_period(objPL_animation->u16P_freq_arr[objPL_animation->u16_idx]);

  if (objPL_animation->u16P_freq_arr[objPL_animation->u16_idx] == LED_TIME)
  {
    gpio_set(CPU_STATUS_GPIO_Port, CPU_STATUS_Pin);
  }
  else
  {
    gpio_clear(CPU_STATUS_GPIO_Port, CPU_STATUS_Pin);
  }

  objPL_animation->u16_idx++;
}

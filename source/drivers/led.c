/**
 * @file led.c
 * @brief LED control driver
 */

#include <stddef.h>

#include <libopencm3/stm32/gpio.h>

#include <utilities/toolbox.h>
#include <pins.h>

#include "timer.h"
#include "led.h"

#define LED_TIME  (200U)
#define LED_PAUSE (1000U)

#define ELEMENT_TIME 500
#define DIT (1*ELEMENT_TIME)
#define DAH (3*ELEMENT_TIME)
#define INTRA (1*ELEMENT_TIME)
#define INTER (3*ELEMENT_TIME)
#define WORD (7*ELEMENT_TIME)

typedef struct
{
  const uint16_t *u16P_freq_arr;
  uint16_t u16_freq_arr_size;
  uint16_t u16_idx;
} led_animation_t;

static void led_timer_callback(void *PL_ctx);

static const uint16_t u16PL_frequency_sequence[] =
{
	LED_TIME,
  LED_TIME,
  LED_TIME,
  LED_PAUSE,
};

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
  
}

void led_cpu_indicate_error(void)
{

}

void led_cpu_indicate_success(void)
{
  timer_start(led_timer_callback, (void*)u16PL_frequency_sequence);
}

static void led_timer_callback(void *PL_ctx)
{
  led_animation_t *objPL_animation = (led_animation_t*)PL_ctx;

  timer_change_period(objPL_animation->u16P_freq_arr[objPL_animation->u16_idx]);

  gpio_toggle(CPU_STATUS_GPIO_Port, CPU_STATUS_Pin);

  objPL_animation->u16_idx++;

  if (objPL_animation->u16_idx == objPL_animation->u16_freq_arr_size)
  {
    objPL_animation->u16_idx = 0;
    timer_stop();
  }
}

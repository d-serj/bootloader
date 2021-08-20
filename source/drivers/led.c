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
  timer_init(led_timer_callback, NULL);
}

void led_cpu_deinit(void)
{
  timer_deinit();
}

void led_cpu_indicate_start(void)
{
  timer_init(led_timer_callback, u16PL_frequency_sequence);


}

void led_cpu_indicate_error(void)
{

}

void led_cpu_indicate_success(void)
{

}

static void led_timer_callback(void *PL_ctx)
{
  static uint8_t u8SL_i = 0;

  timer_change_period(u16PL_frequency_sequence[u8SL_i]);

  gpio_toggle(CPU_STATUS_GPIO_Port, CPU_STATUS_Pin);

  ++u8SL_i;

  if (u8SL_i == ARRAY_SIZE(u16PL_frequency_sequence))
  {
    u8SL_i = 0;
  }
}

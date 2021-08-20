/**
 * @file led.h
 * @brief LED control driver
 */

#ifndef LED_H_
#define LED_H_

#include <pins.h>

void led_cpu_init(void);

void led_cpu_deinit(void);

void led_cpu_indicate_error(void);

void led_cpu_indicate_success(void);

#endif // LED_H_

/**
 * @file pins.h
 */

#ifndef PINS_H_
#define PINS_H_

#include "libopencm3/stm32/gpio.h"

#define CPU_STATUS_Pin           GPIO0
#define CPU_STATUS_GPIO_Port     GPIOC

#define SIGNAL_LED_Pin           GPIO5
#define SIGNAL_LED_GPIO_Port     GPIOC

#define GSM_STATUS_Pin           GPIO0
#define GSM_STATUS_GPIO_Port     GPIOB

#define SIM800_DTR_Pin           GPIO1
#define SIM800_DTR_GPIO_Port     GPIOB

#define SIM800_PWR_OFF_Pin       GPIO7
#define SIM800_PWR_OFF_GPIO_Port GPIOC

#define SIM800_PWR_KEY_Pin       GPIO8
#define SIM800_PWR_KEY_GPIO_Port GPIOC

#define SIM800_DCD_Pin           GPIO2
#define SIM800_DCD_GPIO_Port     GPIOB

#endif /* PINS_H_ */

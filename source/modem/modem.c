/**
 * @file modem_init.c
 * @brief
 */

#include <delay.h>

#include "sim800/sim800.h"

#include "modem.h"

void modem_init(void)
{
  sim800_power_on();
}

void modem_deinit(void)
{
  sim800_power_off();
}

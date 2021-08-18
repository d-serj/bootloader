/**
 * @file sim800.c
 */

 #include <stdbool.h>

#include <libopencm3/stm32/gpio.h>

#include <pins.h>
#include <delay.h>

bool sim800_power_on(void)
{
  if (gpio_get(GSM_STATUS_GPIO_Port, GSM_STATUS_Pin))
  {
    return true;
  }

  gpio_set(SIM800_PWR_OFF_GPIO_Port, SIM800_PWR_OFF_Pin);
  delay(1000);
  gpio_set(SIM800_PWR_KEY_GPIO_Port, SIM800_PWR_KEY_Pin);
  delay(1600);
  gpio_clear(SIM800_PWR_KEY_GPIO_Port, SIM800_PWR_KEY_Pin);

  // Wait till SIM800 will be activated
  delay(3000);

  // Check SIM800 status pin
  return !!gpio_get(GSM_STATUS_GPIO_Port, GSM_STATUS_Pin);
}

bool sim800_power_off(void)
{
  if (!gpio_get(GSM_STATUS_GPIO_Port, GSM_STATUS_Pin))
  {
    return true;
  }

  gpio_set(SIM800_PWR_KEY_GPIO_Port, SIM800_PWR_KEY_Pin);
  delay(1100);
  gpio_clear(SIM800_PWR_KEY_GPIO_Port, SIM800_PWR_KEY_Pin);
  delay(10);

  // Hard power down if still powered on
  if (gpio_get(GSM_STATUS_GPIO_Port, GSM_STATUS_Pin))
  {
    gpio_clear(SIM800_PWR_OFF_GPIO_Port, SIM800_PWR_OFF_Pin);
  }
  
  // Wait till SIM80 will power down
  delay(2000);

  return true;
}

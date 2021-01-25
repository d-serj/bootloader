/**
 * @file sim800.c
 */

 #include <stdbool.h>

#include <libopencm3/stm32/gpio.h>

#include <pins.h>
#include <delay.h>

#include <modem/at_command/at_command.h>

static at_command_t objS_at_cmd = { 0 };

bool sim800_power_on(void)
{
  if (gpio_get(GSM_STATUS_GPIO_Port, GSM_STATUS_Pin))
  {
    return true;
  }

  gpio_set(SIM800_PWR_OFF_GPIO_Port, SIM800_PWR_OFF_Pin);
  delay(10);
  gpio_set(SIM800_PWR_KEY_GPIO_Port, SIM800_PWR_KEY_Pin);
  delay(1100);
  gpio_clear(SIM800_PWR_KEY_GPIO_Port, SIM800_PWR_KEY_Pin);

  // Wait till SIM800 will be activated
  delay(2500);

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

  if ((!!gpio_get(GSM_STATUS_GPIO_Port, GSM_STATUS_Pin)) == false)
  {
    gpio_clear(SIM800_PWR_OFF_GPIO_Port, SIM800_PWR_OFF_Pin);
  }
  else
  {
    return false;
  }

  return true;
}

void sim800_init(void)
{
  //at_command_init(&objS_at_cmd)
  //at_command_write_send("ATE\r\n", )
  //Sim80x_SendAtCommand("ATE1\r\n", 200, 1, "\r\nOK\r\n");								          // Turn echo mode on
}

uint32_t sim800_storage_get_file_size(const char *cPL_file_name)
{
  // Отправить команду по юарт
  // Ждать эхо
  // все что не совпадает с эхо скипаем
  // После эхо начинаем парсить ответ
  
}



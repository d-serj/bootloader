/**
 * @file test_storage.c
 * @brief unit tests for storage.c/h module
 */

#include <stdint.h>
#include <stdbool.h>

#include <unity.h>
#include <fff.h>

#include <utilities/toolbox.h>

#include <stubs/libopencm3/cm3/nvic.h>

#include <storage.h>

extern bool storage_compare_echo(const char *cPL_cmd, uint32_t u32L_cmd_len);

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(usart_send_blocking, uint32_t, uint16_t);
FAKE_VALUE_FUNC(uint16_t, usart_recv, uint32_t);

void test_storage_compare_echo(void)
{
  storage_init();

  const char *cPL_file_name = "test.txt";
  char cPL_buff[128]        = { 0 };
  
  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "AT+FSREAD=%s,%d,%ld,%ld\r\n",
    cPL_file_name, 0, 56, 0);
  
  char cPL_rec_buff[256] = { 0 };

  for (size_t i = 0; i < ARRAY_SIZE(cPL_buff); ++i)
  {
    usart_recv_fake.return_val = cPL_buff[i];
    usart2_isr();
  }
  
  TEST_ASSERT_TRUE(storage_compare_echo(cPL_buff, strlen(cPL_buff)));
}

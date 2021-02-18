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

// Helpers

static void usart_accumulate_data(const uint8_t *u8PL_data, size_t sL_size)
{
  for (size_t i = 0; i < sL_size; ++i)
  {
    usart_recv_fake.return_val = (uint16_t)u8PL_data[i];
    // Call interrupt handler
    usart2_isr();
  }
}

// Tests

void test_storage_compare_echo(void)
{
  storage_init();
  
  RESET_FAKE(usart_send_blocking);
  RESET_FAKE(usart_recv);

  const char *cPL_file_name = "test.txt";
  char cPL_buff[128]        = { 0 };
  
  snprintf(cPL_buff, ARRAY_SIZE(cPL_buff), "AT+FSREAD=%s,%d,%ld,%ld\r\n",
    cPL_file_name, 0, 56, 0);

  usart_accumulate_data((const uint8_t*)cPL_buff, ARRAY_SIZE(cPL_buff));
  
  TEST_ASSERT_TRUE(storage_compare_echo(cPL_buff, strlen(cPL_buff)));
}

void test_storage_get_file_size(void)
{
  RESET_FAKE(usart_send_blocking);
  RESET_FAKE(usart_recv);

  char cPL_buff[128] = { 0 };
  const char *cPL_cmd = "AT+FSFLSIZE=firmware.bin\r\n20\r\n";
  
  strncpy(cPL_buff, cPL_cmd, strlen(cPL_cmd));

  usart_accumulate_data((const uint8_t*)cPL_buff, ARRAY_SIZE(cPL_buff));

  TEST_ASSERT_EQUAL_UINT32(20, storage_get_file_size("firmware.bin"));
  TEST_ASSERT_EQUAL_CHAR('A', usart_send_blocking_fake.arg0_history[0]);
}

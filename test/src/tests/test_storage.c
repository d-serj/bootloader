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

static char cP_buff[128] = { 0 };

extern bool storage_compare_echo(const char *cPL_cmd, uint32_t u32L_cmd_len);

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(usart_send_blocking, uint32_t, uint16_t);
FAKE_VALUE_FUNC(uint16_t, usart_recv, uint32_t);

void setUp(void)
{
  storage_init();
  RESET_FAKE(usart_recv);
  RESET_FAKE(usart_send_blocking);
}

void tearDown(void)
{
  RESET_FAKE(usart_recv);
  RESET_FAKE(usart_send_blocking);
  memset(cP_buff, 0, sizeof(cP_buff));
}

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
  const char *cPL_file_name = "test.txt";
  
  snprintf(cP_buff, ARRAY_SIZE(cP_buff), "AT+FSREAD=%s,%d,%ld,%ld\r\n",
    cPL_file_name, 0, 56, 0);

  usart_accumulate_data((const uint8_t*)cP_buff, ARRAY_SIZE(cP_buff));
  
  TEST_ASSERT_TRUE(storage_compare_echo(cP_buff, strlen(cP_buff)));
}

void test_storage_get_file_size(void)
{
  const char *cPL_cmd = "AT+FSFLSIZE=firmware.bin\r\n+FSFLSIZE: 20\r\n";
  
  strncpy(cP_buff, cPL_cmd, strlen(cPL_cmd));

  usart_accumulate_data((const uint8_t*)cP_buff, strlen(cPL_cmd));

  const uint32_t u32L_result = storage_get_file_size("firmware.bin");

  TEST_ASSERT_EQUAL_UINT32(20, u32L_result);
}

void test_storage_get_chunk(void)
{
  const char *cPL_file_name = "test.txt";
  const uint8_t u8PL_file_data[] = { 0x01, 0xF5, 0xDA, 0xAA };

  snprintf(cP_buff, ARRAY_SIZE(cP_buff), "AT+FSREAD=%s,%d,%ld,%ld\r\n",
    cPL_file_name, 0, ARRAY_SIZE(u8PL_file_data), 0);

  usart_accumulate_data((const uint8_t*)cP_buff, strlen(cP_buff));
  usart_accumulate_data(u8PL_file_data, ARRAY_SIZE(u8PL_file_data));

  uint8_t u8PL_received[128] = { 0 };
  storage_get_chunk("test.txt", 0, u8PL_received, ARRAY_SIZE(u8PL_file_data));
  TEST_ASSERT_EQUAL_UINT8_ARRAY(u8PL_file_data, u8PL_received, ARRAY_SIZE(u8PL_file_data));
}

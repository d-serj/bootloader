/**
 * @file test_usart_driver.c
 * @brief unit tests for usart_driver.c/h
 */

#include <stdint.h>

#include <unity.h>
#include <fff.h>

#include <stubs/libopencm3/cm3/nvic.h>

#include <utilities/toolbox.h>
#include <usart_driver.h>

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(usart_send_blocking, uint32_t, uint16_t);
FAKE_VALUE_FUNC(uint16_t, usart_recv, uint32_t);

static usart_instance_t objS_uart2;

void setUp(void)
{
  usart_setup(&objS_uart2, eUART2);
}

void tearDown(void)
{
  
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

static void usart_receive_accumulated_data(uint8_t *u8PL_data, size_t sL_size)
{
  uint8_t u8L_byte = 0;

  for (size_t i = 0; i < sL_size; ++i)
  {
    if (usart_get_byte(&objS_uart2, &u8L_byte, 1) == 0)
    {
      return;
    }
    
    u8PL_data[i] = u8L_byte;
  }
}

// Tests

void test_usart_send_string(void)
{
  const char *cPL_string_to_send         = "Hello world!";
  const uint16_t u16L_str_to_send_length = strlen(cPL_string_to_send);

  usart_send_string(&objS_uart2, cPL_string_to_send);

  TEST_ASSERT_EQUAL_INT32(u16L_str_to_send_length, usart_send_blocking_fake.call_count);

  for (uint16_t u16L_i = 0; u16L_i <= u16L_str_to_send_length; ++u16L_i)
  {
    TEST_ASSERT_EQUAL_CHAR(cPL_string_to_send[u16L_i], usart_send_blocking_fake.arg1_history[u16L_i]);
  }
}

void test_usart_get_byte(void)
{
  // Receive 'H' from ISR
  usart_recv_fake.return_val = 'H';
  usart2_isr();
  char cL_rec = '\0';
  TEST_ASSERT_TRUE(usart_get_byte(&objS_uart2, (uint8_t*)&cL_rec, 10));
  TEST_ASSERT_EQUAL_CHAR('H', cL_rec);

  // Receive 'e' from ISR
  usart_recv_fake.return_val = 'e';
  usart2_isr();
  TEST_ASSERT_TRUE(usart_get_byte(&objS_uart2, (uint8_t*)&cL_rec, 10));
  TEST_ASSERT_EQUAL_CHAR('e', cL_rec);

  // Timeout
  TEST_ASSERT_EQUAL_UINT8(0, usart_get_byte(&objS_uart2, (uint8_t*)&cL_rec, 1));
  // The function should have been called two times
  TEST_ASSERT_EQUAL_UINT8(2, usart_recv_fake.call_count);

  // Lets accumulate some data
  const uint8_t u8PL_data[] = { 0xAD, 0xDA, 0xFF, 0x01 };
  usart_accumulate_data(u8PL_data, ARRAY_SIZE(u8PL_data));

  // Receive accumulated data
  uint8_t u8PL_buff[50] = { 0 };
  usart_receive_accumulated_data(u8PL_buff, ARRAY_SIZE(u8PL_buff));

  TEST_ASSERT_EQUAL_UINT8_ARRAY(u8PL_data, u8PL_buff, ARRAY_SIZE(u8PL_data));
}

void test_usart_flush(void)
{
  const uint8_t u8PL_data[] = { 0xDE, 0xF5, 0x50, 0x11 };
  usart_accumulate_data(u8PL_data, ARRAY_SIZE(u8PL_data));
  
  uint8_t u8L_byte = 0;

  TEST_ASSERT_TRUE(usart_get_byte(&objS_uart2, &u8L_byte, 1));
  usart_flush(&objS_uart2);
  TEST_ASSERT_FALSE(usart_get_byte(&objS_uart2, &u8L_byte, 1));
}

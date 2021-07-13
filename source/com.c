/**
 * @file com.c
 * @brief  Communication module with host PC. Source file
 */

#include <stdint.h>
#include <stdbool.h>

#include "tinyframe/TinyFrame.h"
#include "utilities/toolbox.h"
#include "comhdlc/minihdlc.h"
#include "drivers/usart_driver.h"
#include "delay.h"

enum eComHdlcCommands
{
  eCmdWriteFile            = 1,
  eCmdWriteFileFinish      = 2,
  eComHdlcAnswer_HandShake = 3,
};

static TinyFrame tf                = { 0 };
static usart_instance_t objS_uart4;
static bool bS_is_master_connected = false;

static void comhdlc_callback(const uint8_t *u8PL_data, uint16_t u16L_data_size);
static void comhdlc_send_byte(uint8_t u8L_byte);
static TF_Result com_listener_handshake(TinyFrame *tf, TF_Msg *msg);
static TF_Result com_listener_file_write(TinyFrame *tf, TF_Msg *msg);

void com_init(void)
{
  usart_setup(&objS_uart4, eUART4);
  minihdlc_init(comhdlc_send_byte, comhdlc_callback);
  TF_InitStatic(&tf, TF_SLAVE);

  TF_AddTypeListener(&tf, eComHdlcAnswer_HandShake, com_listener_handshake);
  TF_AddTypeListener(&tf, eCmdWriteFile, com_listener_file_write);
}

void com_deinit(void)
{
  TF_DeInit(&tf);
  usart_deinit(&objS_uart4);
}

bool com_is_master_connected(uint16_t u16L_timeout)
{
  uint16_t u16L_time = 0;

  while(u16L_time < u16L_timeout)
  {
    if (bS_is_master_connected)
    {
      break;
    }

    ++u16L_time;
    delay(1);
  }

  return bS_is_master_connected;
}

void com_systick_clbk(void *PL_user_data)
{
  UNUSED(PL_user_data);
  TF_Tick(&tf);
}

static TF_Result com_listener_handshake(TinyFrame *tf, TF_Msg *msg)
{
  if (msg->type == eComHdlcAnswer_HandShake)
  {
    bS_is_master_connected = true;
    return TF_CLOSE;
  }

  return TF_NEXT;
}

static TF_Result com_listener_file_write(TinyFrame *tf, TF_Msg *msg)
{
  if (msg->type == eCmdWriteFile)
  {
    bS_is_master_connected = true;
    return TF_CLOSE;
  }

  return TF_NEXT;
}

static void comhdlc_send_byte(uint8_t u8L_byte)
{
  usart_send_raw(&objS_uart4, &u8L_byte, 1);
}

static void comhdlc_callback(const uint8_t *u8PL_data, uint16_t u16L_data_size)
{
  ASSERT(u16L_data_size <= TF_MAX_PAYLOAD_RX);
  TF_Accept(&tf, u8PL_data, u16L_data_size);
}

void TF_WriteImpl(TinyFrame *tf, const uint8_t *buff, uint32_t len)
{
  ASSERT(len <= MINIHDLC_MAX_FRAME_LENGTH);
  minihdlc_send_frame(buff, len);
}

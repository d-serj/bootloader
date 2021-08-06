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
#include "system/assert.h"
#include "delay.h"

#include "storage.h"
#include "storage_sim800.h"

enum eComHdlcCommands
{
  eCmdWriteFile            = 1,
  eCmdWriteFileSize        = 2,
  eCmdWriteFileFinish      = 3,
  eComHdlcAnswer_HandShake = 4,
};

static TinyFrame tf                = { 0 };
static usart_instance_t objS_uart4;
static bool bS_is_master_connected = false;
static bool bS_finished            = false;
static uint32_t u32SL_size_file    = 0;
/** Amount of bytes that were written to the storage */
static uint32_t u32S_bytes_written = 0;
static storage_t *objPS_storage    = NULL;

static void comhdlc_callback(const uint8_t *u8PL_data, uint16_t u16L_data_size);
static void comhdlc_send_byte(uint8_t u8L_byte);
static TF_Result com_listener_handshake(TinyFrame *tf, TF_Msg *msg);
static TF_Result com_listener_file_size(TinyFrame *objPL_tf, TF_Msg *objPL_msg);
static TF_Result com_listener_file_write(TinyFrame *tf, TF_Msg *msg);

void com_init(storage_t *objPL_storage)
{
  bS_finished            = false;
  bS_is_master_connected = false;
  u32SL_size_file        = 0;
  u32S_bytes_written     = 0;

  usart_setup(&objS_uart4, eUART4);
  minihdlc_init(comhdlc_send_byte, comhdlc_callback);
  TF_InitStatic(&tf, TF_SLAVE);

  TF_AddTypeListener(&tf, eComHdlcAnswer_HandShake, com_listener_handshake);
  TF_AddTypeListener(&tf, eCmdWriteFileSize, com_listener_file_size);
  TF_AddTypeListener(&tf, eCmdWriteFile, com_listener_file_write);

  objPS_storage = objPL_storage;
}

void com_deinit(void)
{
  TF_DeInit(&tf);
  usart_deinit(&objS_uart4);
}

void com_run(void)
{
  uint8_t u8L_byte = 0;
  while (usart_get_byte(&objS_uart4, &u8L_byte, 1))
  {
    minihdlc_char_receiver(u8L_byte);
  }
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

    com_run();

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

static TF_Result com_listener_handshake(TinyFrame *objPL_tf, TF_Msg *objPL_msg)
{
  if (objPL_msg->type == eComHdlcAnswer_HandShake)
  {
    static const uint8_t response = eComHdlcAnswer_HandShake;
    objPL_msg->data               = &response;
    objPL_msg->len                = 1;
    TF_Respond(objPL_tf, objPL_msg);

    bS_is_master_connected = true;

    return TF_CLOSE;
  }

  return TF_NEXT;
}

static TF_Result com_listener_file_size(TinyFrame *objPL_tf, TF_Msg *objPL_msg)
{
  if (objPL_msg->type == eCmdWriteFileSize)
  {
    storage_open(objPS_storage, "firmware.bin", eStorageModeCreate);
    u32SL_size_file = *(uint32_t*)objPL_msg->data;

    TF_Respond(objPL_tf, objPL_msg);

    return TF_CLOSE;
  }

  return TF_NEXT;
}

static TF_Result com_listener_file_write(TinyFrame *objPL_tf, TF_Msg *objPL_msg)
{
  TF_Result eL_res = TF_NEXT;

  ASSERT(objPL_tf != NULL);
  ASSERT(objPL_msg != NULL);

  if (objPL_msg->type == eCmdWriteFile)
  {
    uint32_t u32L_bytes_chunk_written = 0;
    ASSERT(objPL_msg->len > 0);
    const int8_t s8L_ret = storage_write(objPS_storage, objPL_msg->data, objPL_msg->len, &u32L_bytes_chunk_written);

    ASSERT(s8L_ret == 0);

    u32S_bytes_written += u32L_bytes_chunk_written;

    if (u32SL_size_file != u32S_bytes_written)
    {
      objPL_msg->len = 0;
      eL_res         = TF_STAY;
    }
    else
    {
      storage_close(objPS_storage);
      bS_finished = true;
      eL_res      = TF_CLOSE;
    }

    TF_Respond(objPL_tf, objPL_msg);
  }

  return eL_res;
}

bool com_file_write_is_finished(void)
{
  com_run();
  return bS_finished;
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

/**
 * @file com.c
 * @brief  Communication module with host PC. Source file
 */

#include <stdint.h>
#include <stdbool.h>

#include <utilities/toolbox.h>
#include <drivers/usart_driver.h>
#include <system/assert.h>
#include <delay.h>
#include <image.h>

#include <storage.h>
#include <storage_sim800.h>

#include "tinyframe/TinyFrame.h"
#include "com.h"

enum eComHdlcCommands
{
  eCmdWriteFile            = 1,
  eCmdWriteFileSize        = 2,
  eCmdWriteFileFinish      = 3,
  eComHdlcAnswer_HandShake = 4,
};

static TinyFrame tf                = { 0 };
static usart_instance_t *objPS_uart4 = NULL;
static bool bS_is_master_connected = false;
static bool bS_finished            = false;
static uint32_t u32SL_size_file    = 0;
/** Amount of bytes that were written to the storage */
static uint32_t u32S_bytes_written = 0;
static storage_t *objPS_storage    = NULL;

static TF_Result com_listener_handshake(TinyFrame *tf, TF_Msg *msg);
static TF_Result com_listener_file_size(TinyFrame *objPL_tf, TF_Msg *objPL_msg);
static TF_Result com_listener_file_write(TinyFrame *tf, TF_Msg *msg);

void com_init(usart_instance_t *objPL_uart)
{
  bS_finished            = false;
  bS_is_master_connected = false;
  u32SL_size_file        = 0;
  u32S_bytes_written     = 0;

  objPS_uart4 = objPL_uart;

  TF_InitStatic(&tf, TF_SLAVE);

  TF_AddTypeListener(&tf, eComHdlcAnswer_HandShake, com_listener_handshake);
  TF_AddTypeListener(&tf, eCmdWriteFileSize, com_listener_file_size);
  TF_AddTypeListener(&tf, eCmdWriteFile, com_listener_file_write);
}

void com_set_storage_to_write_file(storage_t *objPL_storage)
{
  ASSERT(objPL_storage != NULL);
  objPS_storage = objPL_storage;
}

void com_deinit(void)
{
  TF_DeInit(&tf);
}

void com_run(void)
{
  uint8_t u8L_byte = 0;
  while (usart_get_byte(objPS_uart4, &u8L_byte, 1))
  {
    TF_AcceptChar(&tf, u8L_byte);
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
    storage_open(objPS_storage, IMAGE_NAME, eStorageModeCreate);
    u32SL_size_file = *(uint32_t*)objPL_msg->data;

    objPL_msg->len = 0;
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
      eL_res = TF_STAY;
    }
    else
    {
      storage_close(objPS_storage);
      bS_finished = true;
      eL_res      = TF_CLOSE;
    }

    objPL_msg->len = 0;
    TF_Respond(objPL_tf, objPL_msg);
  }

  return eL_res;
}

bool com_file_write_is_finished(void)
{
  com_run();
  return bS_finished;
}

void TF_WriteImpl(TinyFrame *tf, const uint8_t *u8PL_buff, uint32_t u32L_len)
{
  ASSERT(u32L_len < TF_SENDBUF_LEN);
  
  if (objPS_uart4)
  {
    usart_send_raw(objPS_uart4, u8PL_buff, u32L_len);
  }
}

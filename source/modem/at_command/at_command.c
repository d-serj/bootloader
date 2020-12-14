/**
 * @file at_command.c
 * @brief AT commands sender and parser. Source file
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <drivers/usart.h>
#include <delay.h>

#include "at_command.h"

void at_command_init(at_command_t *objPL_at_cmd,
  char *cPL_send_buf, uint32_t u32L_send_buf_size, char *cPL_rec_buf)
{
  objPL_at_cmd->cP_send_buf    = cPL_send_buf;
  objPL_at_cmd->u32_buf_size   = u32L_send_buf_size;
  objPL_at_cmd->cP_receive_buf = cPL_rec_buf;
  objPL_at_cmd->u32_timeout    = 0;
  objPL_at_cmd->e_cmd_state    = eATcmdStateReady;
}

at_cmd_state_t at_command_check_buf(const char *cPL_buf)
{
  if (u8_uart_data_received)
  {
    u8_uart_data_received = 0;
    // if answer is ok
    if (strstr(cPL_buf, "OK\r\n") != NULL)
    {
      return eATcmdStateOk;
    }

    // Response "ERROR" or something else
    return eATcmdStateErr;
  }

  return eATcmdStateWait;
}

at_cmd_res_t at_command_send(at_command_t *objPL_at_cmd, uint32_t u32L_timeout)
{
  assert(objPL_at_cmd != NULL);

  switch (objPL_at_cmd->e_cmd_state)
  {
  case eATcmdStateReady:
    usart_send_string((const uint8_t*)objPL_at_cmd->cP_send_buf);
    objPL_at_cmd->e_cmd_state = eATcmdStateWait;
    break;

  case eATcmdStateWait:
    delay(10);
    objPL_at_cmd->u32_timeout += 10;
    objPL_at_cmd->e_cmd_state = 
      at_command_check_buf(objPL_at_cmd->cP_receive_buf);
    if ((objPL_at_cmd->e_cmd_state != eATcmdStateOk)
      && (objPL_at_cmd->u32_timeout >= u32L_timeout)) 
    {
      return eATcmdResultTimeout;
    }
    break;

  case eATcmdStateOk:
    usart_clear_rx_buf();
    objPL_at_cmd->e_cmd_state = eATcmdStateReady;
    return eATcmdResultOk;

  case eATcmdStateErr:
    return eATcmdResultErr;
  
  default:
    assert(0);
    break;
  }

  return eATcmdResultBusy;
}

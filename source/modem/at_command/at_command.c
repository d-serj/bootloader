/**
 * @file at_command.c
 * @brief AT commands sender and parser. Source file
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <drivers/usart_driver.h>
#include <delay.h>

#include "at_command.h"

void at_command_init(at_command_t *objPL_at_cmd,
  char *cPL_send_buf, uint32_t u32L_send_buf_size, char *cPL_rec_buf)
{
  objPL_at_cmd->cP_sent_cmd    = cPL_send_buf;
  objPL_at_cmd->u32_buf_size   = u32L_send_buf_size;
  objPL_at_cmd->u32_timeout    = 0;
}

at_cmd_state_t at_command_check_buf(const char *cPL_buf)
{
  // if answer is ok
  if (strstr(cPL_buf, "OK\r\n") != NULL)
  {
    return eATcmdStateOk;
  }

  return eATcmdStateWait;
}

void at_command_write_send(at_command_t *objPL_at_cmd, uint32_t u32L_timeout)
{
  assert(objPL_at_cmd != NULL);
  
  //usart_send_string(&obj_(const uint8_t*)objPL_at_cmd->cP_sent_cmd);

  objPL_at_cmd->u32_timeout = u32L_timeout;
}

uint8_t *at_command_receive_response(at_command_t *objPL_at_cmd)
{
  while (objPL_at_cmd->u32_timeout)
  {

    delay(1);
    objPL_at_cmd->u32_timeout--;
  }
}

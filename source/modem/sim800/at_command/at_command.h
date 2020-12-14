/**
 * @file at_command.h
 * @brief AT commands sender and parser
 */

#ifndef AT_COMMAND_H_
#define AT_COMMAND_H_

#include <stdint.h>
#include <stdbool.h>

enum atCommandResult
{
  eATcmdResultOk      = 0,
  eATcmdResultBusy    = 1,
  eATcmdResultErr     = 2,
  eATcmdResultTimeout = 3,
};
typedef enum atCommandResult at_cmd_res_t;

enum atCommandState
{
  eATcmdStateOk    = 0,
  eATcmdStateReady = 1,
  eATcmdStateWait  = 2,
  eATcmdStateErr   = 3,
};
typedef enum atCommandState at_cmd_state_t;

struct atCommand
{
  char *cP_send_buf;
  const char *cP_receive_buf;
  uint32_t u32_buf_size;   // 
  uint32_t u32_timeout;          // AT command timeout
  at_cmd_state_t e_cmd_state;  
};
typedef struct atCommand at_command_t;

void at_command_init(at_command_t *objPL_at_cmd,
  char *cPL_send_buf, uint32_t u32L_send_buf_size, char *cPL_rec_buf);

at_cmd_res_t at_command_send(at_command_t *objPL_at_cmd, uint32_t u32L_timeout);

#endif // AT_COMMAND_H_

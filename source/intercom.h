/**
 * @file intercom.h
 * @brief List of commands for intercommunication between main app and bootloader
 */

#ifndef INTERCOM_H_
#define INTERCOM_H_

enum eInterCom
{
  eInterCom_Empty    = 0,
  eInterCom_Bootload = (1 << 0),
  eInterCom_Force    = (1 << 1),
};

#endif // INTERCOM_H_

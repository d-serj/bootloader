/**
 * @file main.c
 * @brief Main file of the program
 */

#include "bootloader.h"

int main(void)
{
  bootloader();

  for(;;)
    ;

  return 0;
}

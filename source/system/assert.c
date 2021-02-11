/**
 * @file assert.c
 * @brief Asserts for ARM processors. Source file
 */


#if defined(DEBUG)

#include <stdint.h>

#include "assert.h"

void __attribute__((weak, noreturn)) assert_handler(const uint32_t *pc, const uint32_t *lr)
{
  (void)pc;
  (void)lr;

  __asm("bkpt 5");

  while(1)
    {};
}

#endif // DEBUG

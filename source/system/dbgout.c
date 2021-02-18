/**
 * @file dbgout.c
 * @brief Debug IO functions implementations
 */


#include <stm32f105xc.h>

#ifdef DEBUG

volatile int32_t ITM_RxBuffer;

void dbgout_init(void)
{
  // 	ITMENA. Enable ITM
  ITM->TCR |= ITM_TCR_ITMENA_Msk;

  // Enable stimulus port 0 
  ITM->TER |= (1 << 0);
}

void __io_putchar(int ch)
{
  ITM_SendChar((uint32_t)ch);
}
#endif

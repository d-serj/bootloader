/**
 * @file main.c
 * @brief Main file of the program
 */

#include <libopencm3/stm32/rcc.h>

int main(void)
{
  rcc_clock_setup_in_hse_8mhz_out_24mhz();
  

  return 0;
}

/**
 * @file bootloader.c
 * @brief 
 */

#include <stdint.h>
#include <stddef.h>

#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/vector.h>
#include <libopencmsis/core_cm3.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/f1/bkp.h>

#include "system/system.h"
#include "drivers/usart_driver.h"
#include "drivers/bkp.h"
#include "comhdlc/com.h"
#include "delay.h"
#include "image.h"
#include "storage/storage.h"
#include "storage/storage_internal.h"
#include "storage/storage_sim800.h"


#include "memory_map.h"
#include "bootloader.h"

static usart_instance_t objS_uart2;
static usart_instance_t objS_uart4;

static void image_start(uint32_t u32L_vector_addr) __attribute__((noreturn));

static inline __attribute__((always_inline)) void __set_MSP(volatile uint32_t topOfMainStack)
{
  //  Set the stack pointer.
  asm volatile("msr msp, %0" : : "r" (topOfMainStack) : );
}

void bootloader(void)
{
    bool image_could_start = false;
  const uint32_t u32L_main_app_addr = (uint32_t)&__app_start__;

  system_init();
  systick_init(com_systick_clbk, NULL);

  usart_setup(&objS_uart2, eUART2);
  usart_setup(&objS_uart4, eUART4);

  // 1. Check flags
  const uint16_t u16L_main_app_cmd = bkp_read(BKP_DR1);

  // 2. Try to connect to the host PC. Wait 10 seconds for a connection
  com_init(&objS_uart4);
  if (com_is_master_connected(1000))
  {
    storage_t *const objPL_storage_sim800 = storage_sim800_init_static(&objS_uart2);
    com_set_storage_to_write_file(objPL_storage_sim800);

    while (com_file_write_is_finished() == false)
      ;
  }

  // 3. Load app or go to update
  image_t objL_image = { 0 };

  storage_t *const objPL_storage_sim800 = storage_sim800_init_static(&objS_uart2);
  if (image_open(&objL_image, objPL_storage_sim800, IMAGE_NAME) == 0)
  {
    if (image_validate(&objL_image))
    {
      const int8_t s8L_ret = image_copy(&objL_image, storage_internal_init_static(u32L_main_app_addr));
      image_could_start    = (s8L_ret == 0);
    }
    else
    {
      image_close(&objL_image);
    }
  }

  // Deinit all the stuff
  com_deinit();
  storage_sim800_deinit();
  delay(2000);

  systick_deinit();
  usart_deinit(&objS_uart2);
  usart_deinit(&objS_uart4);
  system_deinit();
  
  if (image_could_start)
  {
    image_start(objL_image.obj_img_hdr.u32_vector_addr);
  }
}

void image_start(uint32_t u32L_vector_addr)
{
  const vector_table_t *objPL_vector =
    (const vector_table_t *)u32L_vector_addr;

  // Disable interrupts before modifing VTOR
  __disable_irq();
  // Ensure all memory operations are finished before modifing VTOR
  asm("dmb");
  SCB_VTOR = (uint32_t)objPL_vector & 0xFFFFF;
  asm("dsb");
  __enable_irq();

  __set_MSP((volatile uint32_t)objPL_vector->initial_sp_value);
  // Call the reset handler
  // (the construct below is 'pointer to a pointer to a function that takes no arguments and returns void')
  void (*reset_handler)(void) = (void *)*(volatile uint32_t *)(u32L_vector_addr + 4);
  reset_handler();
  __builtin_unreachable();
}

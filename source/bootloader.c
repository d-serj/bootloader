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
#include "drivers/led.h"
#include "comhdlc/com.h"
#include "delay.h"
#include "image.h"
#include "storage/storage.h"
#include "storage/storage_internal.h"
#include "storage/storage_sim800.h"

#include "intercom.h"
#include "memory_map.h"
#include "bootloader.h"

static usart_instance_t objS_uart2;
static usart_instance_t objS_uart4;

static void image_start(uint32_t u32L_vector_addr) __attribute__((noreturn));
static bool image_flash(image_t *objPL_image, storage_t *objPL_src, storage_t *objPL_dst);

static inline __attribute__((always_inline)) void __set_MSP(volatile uint32_t topOfMainStack)
{
  //  Set the stack pointer.
  asm volatile("msr msp, %0" : : "r" (topOfMainStack) : );
}

void bootloader(void)
{
  const uint32_t u32L_main_app_addr = (uint32_t)&__app_start__;
  bool bL_image_could_start         = false;
  image_t objL_image                = { 0 };

  system_init();
  systick_init(com_systick_clbk, NULL);

  usart_setup(&objS_uart2, eUART2);
  usart_setup(&objS_uart4, eUART4);

  led_cpu_init();

  led_cpu_indicate_start();

  // 1. Check main app stored on internal flash
  storage_t *objPL_internal_flash = storage_internal_init_static(u32L_main_app_addr);
  if ((image_open(&objL_image, objPL_internal_flash, IMAGE_NAME) == 0)
    && image_validate(&objL_image))
  {
    bL_image_could_start = true;
  }

  // 2. Check commands from the main app
  const uint16_t u16L_main_app_cmd = bkp_read(BKP_DR1);

  // 3. Go to update if command
  if (u16L_main_app_cmd == eInterCom_Bootload)
  {
    image_close(&objL_image);
    storage_t *objPL_src = storage_sim800_init_static(&objS_uart2);
    bL_image_could_start = image_flash(&objL_image, objPL_src, objPL_internal_flash);
  }
  // 4. Try to connect to the host PC. Wait 1 seconds for a connection
  else
  {
    com_init(&objS_uart4);
    // If Host PC is connected received the file and write it to external storage
    if (com_is_master_connected(1000))
    {
      storage_t *const objPL_storage_sim800 = storage_sim800_init_static(&objS_uart2);
      com_set_storage_to_write_file(objPL_storage_sim800);
      // Currently supported only file transferring
      while (com_file_write_is_finished() == false)
        ;

      image_close(&objL_image);
      bL_image_could_start = image_flash(&objL_image, objPL_storage_sim800, objPL_internal_flash);
    }

    com_deinit();
  }


  led_cpu_deinit();
  // Deinit all the stuff
  storage_sim800_deinit();
  systick_deinit();
  usart_deinit(&objS_uart2);
  usart_deinit(&objS_uart4);
  system_deinit();
  
  if (bL_image_could_start)
  {
    image_start(objL_image.obj_img_hdr.u32_vector_addr);
  }
  else
  {
    led_cpu_indicate_error();
  }
}

static bool image_flash(image_t *objPL_image, storage_t *objPL_src, storage_t *objPL_dst)
{
  bool bL_image_could_start = false;
  if (image_open(objPL_image, objPL_src, IMAGE_NAME) == 0)
  {
    if (image_validate(objPL_image))
    {
      const int8_t s8L_ret = image_copy(objPL_image, objPL_dst);
      bL_image_could_start = (s8L_ret == 0);
    }
    else
    {
      image_close(objPL_image);
    }
  }

  return bL_image_could_start;
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

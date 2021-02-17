/**
 * @file gpio.c
 * @brief stubs for gpio.h/c driver
 */

#include "gpio.h"

void gpio_set_mode(uint32_t gpioport, uint8_t mode, uint8_t cnf,
		   uint16_t gpios)
{

}

void gpio_set_eventout(uint8_t evoutport, uint8_t evoutpin)
{

}

void gpio_primary_remap(uint32_t swjenable, uint32_t maps)
{

}

void gpio_secondary_remap(uint32_t maps)
{

}

void gpio_set(uint32_t gpioport, uint16_t gpios)
{

}

void gpio_clear(uint32_t gpioport, uint16_t gpios)
{

}

uint16_t gpio_get(uint32_t gpioport, uint16_t gpios)
{
	return 1;
}

void gpio_toggle(uint32_t gpioport, uint16_t gpios)
{
 
}

uint16_t gpio_port_read(uint32_t gpioport)
{

}

void gpio_port_write(uint32_t gpioport, uint16_t data)
{

}

void gpio_port_config_lock(uint32_t gpioport, uint16_t gpios)
{
  
}

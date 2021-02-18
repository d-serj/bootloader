/**
 * @file nvic.c
 * @brief stubs for nvic.c/h driver
 */

#include <stdint.h>

#include "nvic.h"

void nvic_enable_irq(uint8_t irqn)
{
}
void nvic_disable_irq(uint8_t irqn)
{
}
uint8_t nvic_get_pending_irq(uint8_t irqn)
{
}
void nvic_set_pending_irq(uint8_t irqn)
{
}
void nvic_clear_pending_irq(uint8_t irqn)
{
}
uint8_t nvic_get_irq_enabled(uint8_t irqn)
{
}
void nvic_set_priority(uint8_t irqn, uint8_t priority)
{
}

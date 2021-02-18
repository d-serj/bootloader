/**
 * @file rcc.c
 * @brief stubs for rcc.c/h driver
 */

#include <stdint.h>

#include "rcc.h"

void rcc_osc_ready_int_clear(enum rcc_osc osc)
{

}

void rcc_osc_ready_int_enable(enum rcc_osc osc)
{

}

void rcc_osc_ready_int_disable(enum rcc_osc osc)
{

}

int rcc_osc_ready_int_flag(enum rcc_osc osc)
{

}

void rcc_css_int_clear(void)
{

}

int rcc_css_int_flag(void)
{

}

void rcc_osc_on(enum rcc_osc osc)
{

}

void rcc_osc_off(enum rcc_osc osc)
{

}

void rcc_css_enable(void)
{
}
void rcc_css_disable(void)
{
}
void rcc_set_sysclk_source(uint32_t clk)
{
}
void rcc_set_pll_multiplication_factor(uint32_t mul)
{
}
void rcc_set_pll2_multiplication_factor(uint32_t mul)
{
}
void rcc_set_pll3_multiplication_factor(uint32_t mul)
{
}
void rcc_set_pll_source(uint32_t pllsrc)
{
}
void rcc_set_pllxtpre(uint32_t pllxtpre)
{
}
uint32_t rcc_rtc_clock_enabled_flag(void)
{
}
void rcc_enable_rtc_clock(void)
{
}
void rcc_set_rtc_clock_source(enum rcc_osc clock_source)
{
}
void rcc_set_adcpre(uint32_t adcpre)
{
}
void rcc_set_ppre2(uint32_t ppre2)
{
}
void rcc_set_ppre1(uint32_t ppre1)
{
}
void rcc_set_hpre(uint32_t hpre)
{
}
void rcc_set_usbpre(uint32_t usbpre)
{
}
void rcc_set_prediv1(uint32_t prediv)
{
}
void rcc_set_prediv2(uint32_t prediv)
{
}
void rcc_set_prediv1_source(uint32_t rccsrc)
{
}
uint32_t rcc_system_clock_source(void)
{
}
void rcc_clock_setup_in_hsi_out_64mhz(void)
{
}
void rcc_clock_setup_in_hsi_out_48mhz(void)
{
}
void rcc_clock_setup_in_hsi_out_24mhz(void)
{
}
void rcc_clock_setup_in_hse_8mhz_out_24mhz(void)
{
}
void rcc_clock_setup_in_hse_8mhz_out_72mhz(void)
{
}
void rcc_clock_setup_in_hse_12mhz_out_72mhz(void)
{
}
void rcc_clock_setup_in_hse_16mhz_out_72mhz(void)
{
}
void rcc_clock_setup_in_hse_25mhz_out_72mhz(void)
{
}
void rcc_backupdomain_reset(void)
{
}

void rcc_peripheral_enable_clock(volatile uint32_t *reg, uint32_t en)
{
}
void rcc_peripheral_disable_clock(volatile uint32_t *reg, uint32_t en)
{
}
void rcc_peripheral_reset(volatile uint32_t *reg, uint32_t reset)
{
}
void rcc_peripheral_clear_reset(volatile uint32_t *reg, uint32_t clear_reset)
{
}

void rcc_periph_clock_enable(enum rcc_periph_clken clken)
{
}
void rcc_periph_clock_disable(enum rcc_periph_clken clken)
{
}
void rcc_periph_reset_pulse(enum rcc_periph_rst rst)
{
}
void rcc_periph_reset_hold(enum rcc_periph_rst rst)
{
}
void rcc_periph_reset_release(enum rcc_periph_rst rst)
{
}

void rcc_set_mco(uint32_t mcosrc)
{
}
void rcc_osc_bypass_enable(enum rcc_osc osc)
{
}
void rcc_osc_bypass_disable(enum rcc_osc osc)
{
}

/**
 * Is the given oscillator ready?
 * @param osc Oscillator ID
 * @return true if the hardware indicates the oscillator is ready.
 */
bool rcc_is_osc_ready(enum rcc_osc osc)
{
}

/**
 * Wait for Oscillator Ready.
 * Block until the hardware indicates that the Oscillator is ready.
 * @param osc Oscillator ID
 */
void rcc_wait_for_osc_ready(enum rcc_osc osc)
{
}
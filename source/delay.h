/**
 * @file delay.h
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

/** Callback to be called from Sys_tick handler */
typedef void (*systick_clbk_t)(void *PL_user_data);

/**
 * @brief Init systick for delay purposes
 */
void systick_init(systick_clbk_t objPL_callback, void *PL_user_data);

/**
 * @brief Deinit systick
 */
void systick_deinit(void);

/**
 * @brief Get the current value of the millis counter
 * @return current value of the millis counter
 */
uint64_t millis(void);

/**
 * @brief Delay a given number of milliseconds in a blocking manner
 * @param u64L_duration .. the value of delay
 */
void delay(uint64_t u64L_duration);


#endif /* DELAY_H_ */

/**
 * @file timer.h
 */

#ifndef TIMER_H_
#define TIMER_H_

/**
 * @brief Callback to be called inside the timer interrupt routine
 * @param PL_context .. user context to be passed inside the callback
 */
typedef void (*timer_callback_t)(void *PL_context);

/**
 * @brief Initialize TIM2 to be used as tick timer
 * @param objPL_callback .. user callback to be called inside the TIM2 interrupt routine
 * @param PL_user_context .. user context to be passed into objPL_callback
 */
void timer_init(timer_callback_t objPL_callback, void *PL_user_context);

/**
 * @brief Deinitialize timer 2
 */
void timer_deinit(void);

/**
 * @brief Change timer period
 * @param u16L_new_period .. new period to be set
 */
void timer_change_period(uint16_t u16L_new_period);


#endif // TIMER_H_

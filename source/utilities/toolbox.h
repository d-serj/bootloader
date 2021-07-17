/**
 * @file toolbox.h
 *
 */

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

/**
 * @brief Macro to set the result of computation or variable as unused
 */
#define UNUSED(x) ((void)x)

/**
 * @brief Number of elements within an array
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#endif // TOOLBOX_H_

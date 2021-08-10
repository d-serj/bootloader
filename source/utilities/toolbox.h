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

/**
 * @brief Return the smallest value out of two possible values
 */
#ifndef MIN
#define MIN(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _b : _a; })
#endif

/**
 * @brief Return the largest value out of two possible values
 */
#ifndef MAX
#define MAX(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })
#endif

#endif // TOOLBOX_H_

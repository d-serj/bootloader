/**
 * @file assert.h
 * @brief Asserts for ARM processors. Header file
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#if defined(DEBUG)

  #include <stdint.h>

  /**
   * @brief Assert handler
   * @param pc .. Program counter
   * @param lr .. Link register
   */
  void assert_handler(const uint32_t *pc, const uint32_t *lr);

  #define GET_LR() __builtin_return_address(0)
  // This is ARM and GCC specific syntax
  #define GET_PC(_a) __asm volatile ("mov %0, pc" : "=r" (_a))

  #define ASSERT_RECORD()        \
    do                           \
    {                            \
      void *pc;                  \
      GET_PC(pc);                \
      const void *lr = GET_LR(); \
      assert_handler(pc, lr);    \
    } while (0)

  #define ASSERT(exp)            \
    do                           \
    {                            \
      if (!(exp))                \
      {                          \
        ASSERT_RECORD();         \
      }                          \
    } while (0)

#else
  #if defined(__linux__) || defined(_WIN32)
    #include <assert.h>
    #define ASSERT(exp) assert(exp)
  #else
    #define ASSERT(exp) ((void)0U)
  #endif // defined(__linux) || defined(_WIN32)
#endif // DEBUG

#endif // ASSERT_H_

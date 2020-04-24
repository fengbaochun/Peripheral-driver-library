#ifndef __CORE_DELAY_H
#define __CORE_DELAY_H


#include "stm32f4xx.h"
#include <stdbool.h>

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#ifdef DWT_BASE
uint32_t dwt_init(void);

#ifdef __cplusplus
extern "C" {
#endif

void dwt_access(bool ena);

static inline uint32_t dwt_max_sec(void)
{
  return (UINT32_MAX / SystemCoreClock);
};

static inline uint32_t dwt_max_msec(void)
{
  return (UINT32_MAX / (SystemCoreClock / 1000));
};

static inline uint32_t dwt_max_usec(void)
{
  return (UINT32_MAX / (SystemCoreClock / 1000000));
};

static inline uint32_t dwt_getCycles(void)
{
  return (DWT->CYCCNT);
};

extern void delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

extern uint32_t dwt_init(void);
extern void delay_us(uint32_t us)  ;
extern void delay_ms(uint32_t ms)  ;
extern void delay_s(uint32_t s)    ;

#endif /* DWT_BASE */
#endif /* _DWT_H_ */

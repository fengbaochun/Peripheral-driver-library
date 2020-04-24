#include "./delay/core_delay.h"   


#ifdef DWT_BASE
#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  dwt_init 初始化
  * @retval 无
  */
uint32_t dwt_init(void)
{

  /* Enable use of DWT */
  if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  }

  /* Unlock */
  dwt_access(true);

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk;

  /* 3 NO OPERATION instructions */
  __asm volatile(" nop      \n\t"
                 " nop      \n\t"
                 " nop      \n\t");

  /* Check if clock cycle counter has started */
  return (DWT->CYCCNT) ? 0 : 1;
}

/**
  * @brief  断言
  * @retval 无
  */
void dwt_access(bool ena)
{
#if (__CORTEX_M == 0x07U)
  /*
   * Define DWT LSR mask which is (currentuly) not defined by the CMSIS.
   * Same as ITM LSR one.
   */
#if !defined DWT_LSR_Present_Msk
#define DWT_LSR_Present_Msk ITM_LSR_Present_Msk
#endif
#if !defined DWT_LSR_Access_Msk
#define DWT_LSR_Access_Msk ITM_LSR_Access_Msk
#endif
  uint32_t lsr = DWT->LSR;

  if ((lsr & DWT_LSR_Present_Msk) != 0) {
    if (ena) {
      if ((lsr & DWT_LSR_Access_Msk) != 0) { //locked
        DWT->LAR = 0xC5ACCE55;
      }
    } else {
      if ((lsr & DWT_LSR_Access_Msk) == 0) { //unlocked
        DWT->LAR = 0;
      }
    }
  }
#else /* __CORTEX_M */
  UNUSED(ena);
#endif /* __CORTEX_M */
}


/**
  * @brief  us级延时
  * @param  us   延时时间
  * @retval 无
  */
void delay_us(uint32_t us)
{
#if defined(DWT_BASE) && !defined(DWT_DELAY_DISABLED)
  int32_t start  = dwt_getCycles();
  int32_t cycles = us * (SystemCoreClock / 1000000);

  while ((int32_t)dwt_getCycles() - start < cycles);
#endif
}


/**
  * @brief  ms级延时
  * @param  ms   延时时间
  * @retval 无
  */
void delay_ms(uint32_t ms)
{
#if defined(DWT_BASE) && !defined(DWT_DELAY_DISABLED)
  int32_t start  = dwt_getCycles();
  int32_t cycles = ms * (SystemCoreClock / 1000);

  while ((int32_t)dwt_getCycles() - start < cycles);
#endif
}


/**
  * @brief  s级延时
  * @param  s   延时时间
  * @retval 无
  */
void delay_s(uint32_t s)
{
#if defined(DWT_BASE) && !defined(DWT_DELAY_DISABLED)
  int32_t start  = dwt_getCycles();
  int32_t cycles = s * (SystemCoreClock / 1);

  while ((int32_t)dwt_getCycles() - start < cycles);
#endif
}


#ifdef __cplusplus
}
#endif

#endif

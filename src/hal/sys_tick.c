#include "sys_tick.h"
#include <ch32v00x.h>

static volatile uint32_t sys_time_ms = 0;

__attribute__((interrupt("WCH-Interrupt-fast"))) void SysTick_Handler(void);

void SysTick_Handler(void) {
  sys_time_ms++;
  SysTick->SR = 0;
}

void SysTick_Configure(void) {
  SysTick->SR = 0; // Clear status register
  SysTick->CNT = 0; // Clear counter register
  
  NVIC_SetPriority(SysTicK_IRQn, 0x00); // High priority
  NVIC_EnableIRQ(SysTicK_IRQn);

  SysTick->CMP = (SystemCoreClock / 1000) - 1; // 1ms

  /**
   * bit 0 - Start the system counter STK
   * bit 1 - Enabling counter interrupts
   * bit 2 - STCLK - HCLK for time base
   * bit 3 - STRE - Count up to the comparison value and start counting from 0 again
   */
  SysTick->CTLR |= (1<<0) | (1<<1) | (1<<2) | (1<<3);
}

uint32_t SysTick_GetTicks(void) {
  uint32_t ms1, ms2, cnt;

  do {
    ms1 = sys_time_ms;
    cnt = SysTick->CNT;
    ms2 = sys_time_ms;
  } while (ms1 != ms2);

  uint32_t ticks = (ms1 * ((SystemCoreClock / 1000 ))) + cnt;

  return ticks;
}

uint32_t SysTick_GetSysTimeMS(void) {
  return sys_time_ms;
}

void SysTick_DelayMS(uint32_t ms) {
  uint32_t start = SysTick_GetSysTimeMS();
  while ((SysTick_GetSysTimeMS() - start) < ms);
}

void SysTick_DelayUS(uint32_t us) {
  uint32_t desired_ticks = us * (SystemCoreClock / 1000000);
  uint32_t start_ticks = SysTick_GetTicks();
  
  while ((SysTick_GetTicks() - start_ticks) < desired_ticks);
}
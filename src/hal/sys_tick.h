#pragma once

#include <stdint.h>

void SysTick_Configure(void);
uint32_t SysTick_GetTicks(void);
uint32_t SysTick_GetSysTimeMS(void);
void SysTick_DelayMS(uint32_t ms);
void SysTick_DelayUS(uint32_t us);
#pragma once

#include "ch32v00x_gpio.h"

#define LED_LIVE_BIT_PORT GPIOD
#define LED_LIVE_BIT_PIN GPIO_Pin_4

void LED_LiveBitConfigure(void);
void LED_LiveBitRun(void);
#include "led.h"
#include <stdio.h>
#include "ch32v00x_gpio.h"
#include "ch32v00x_rcc.h"

void LED_LiveBitConfigure(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

  GPIO_InitTypeDef GPIOD_InitStructure = {0};

  GPIOD_InitStructure.GPIO_Pin = LED_LIVE_BIT_PIN;
  GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIOD_InitStructure);
}

void LED_LiveBitRun(void) {
  GPIOD->OUTDR ^= LED_LIVE_BIT_PIN;
  // printf("test SDI\r\n");
}
#include "ppm.h"
#include <stdint.h>
#include "ch32v00x_gpio.h"
#include "ch32v00x_tim.h"
#include "utils/scale.h"

#define SERVO_FREQ    60
#define TICKS_PER_SEC 1000000

#define TIM_PRESCALER    ((SystemCoreClock / TICKS_PER_SEC) - 1)
#define TIM_PERIOD       ((TICKS_PER_SEC / SERVO_FREQ) - 1)

void PPM_Configure(void) {
  GPIO_InitTypeDef        GPIOD_InitStructure = {0};
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
  TIM_OCInitTypeDef       TIM_OCInitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOD, ENABLE);

  GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOD, &GPIOD_InitStructure);

  TIM_TimeBaseInitStructure.TIM_Period = TIM_PERIOD;
  TIM_TimeBaseInitStructure.TIM_Prescaler = TIM_PRESCALER;
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1500;  
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
}

void PPM_SetDutyCycle(uint8_t value) {
  uint16_t pulse = scale_with_params(value, 100, 0, 1000, 2000);
  TIM_SetCompare1(TIM1, pulse); 
}
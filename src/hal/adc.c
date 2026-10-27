#include "adc.h"
#include "ch32v00x.h"
#include "ch32v00x_adc.h"
#include "ch32v00x_gpio.h"
#include "ch32v00x_rcc.h"
#include "core_riscv.h"

ADC_RawValues raw_values;

void ADC_Configure(void) {
  ADC_InitTypeDef ADC_InitStructure = {0};
  GPIO_InitTypeDef GPIOC_InitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

  GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIOC_InitStructure);

  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_241Cycles);

  ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);

  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while (ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while (ADC_GetCalibrationStatus(ADC1));
}

void ADC_Conversion(void) {
  if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET) return;

  uint16_t result = ADC_GetConversionValue(ADC1);
  raw_values.throttle = result;

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

ADC_RawValues *ADC_GetRawValues(void) { return &raw_values; }
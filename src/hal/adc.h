#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct ADC_RawValues {
  volatile uint16_t throttle;
} ADC_RawValues;

// #define SETTING_VREF 2.58f  // V

// #define ADC_VoltageInCalc(raw) ((raw * SETTING_VREF) / 1024.0f)

void ADC_Configure(void);

void ADC_Conversion(void);

// void ADC_SelectChannel(uint8_t channel);

// float ADC_FilterEMA(uint16_t raw, float prev, float alpha);

ADC_RawValues *ADC_GetRawValues(void);

/**
 * @brief Scale with parameters
 *
 * @param raw
 * @param raw_max
 * @param raw_min
 * @param scale_min
 * @param scale_max
 * @return uint16_t
 */
// uint16_t ADC_SCP(uint16_t raw, uint16_t raw_max, uint16_t raw_min, uint16_t scale_min, uint16_t scale_max);
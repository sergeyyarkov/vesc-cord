#include "throttle.h"
#include "hal/adc.h"
#include "utils/filter_ema.h"

uint16_t Throttle_GetFilteredValue(void) {
  uint16_t adc_raw_throttle = ADC_GetRawValues()->throttle;
  static float filtered_val = -1.0f;
  float alpha;

  // Failure check
  if (adc_raw_throttle < 100) {
    filtered_val = 0;
    return 0;
  }

  if (filtered_val < 0) filtered_val = adc_raw_throttle;  // init with raw value

  if (adc_raw_throttle < filtered_val) {
    alpha = 1.0f;
  } else {
    alpha = 0.25f;
  }

  filtered_val = filter_ema(adc_raw_throttle, filtered_val, alpha);

  return (uint16_t)filtered_val;
}
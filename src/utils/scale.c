#include "scale.h"

uint16_t scale_with_params(uint16_t raw, uint16_t raw_max, uint16_t raw_min, uint16_t scale_min, uint16_t scale_max) {
  if (raw <= raw_min) return scale_min;
  if (raw >= raw_max) return scale_max;

  uint32_t scaled = (uint32_t)(raw - raw_min) * (scale_max - scale_min);
  scaled /= (raw_max - raw_min);
  scaled += scale_min;

  return scaled;
}
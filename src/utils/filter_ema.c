#include "filter_ema.h"

inline float filter_ema(uint16_t raw, float prev, float alpha) { 
  return alpha * raw + (1.0f - alpha) * prev; 
}
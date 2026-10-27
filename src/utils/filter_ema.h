#pragma once

#include <stdint.h>

float filter_ema(uint16_t raw, float prev, float alpha);
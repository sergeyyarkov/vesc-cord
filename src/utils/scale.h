#pragma once

#include <stdint.h>

uint16_t scale_with_params(uint16_t raw, uint16_t raw_max, uint16_t raw_min, uint16_t scale_min, uint16_t scale_max);
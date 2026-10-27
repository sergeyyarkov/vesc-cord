#pragma once

#include <stdint.h>

uint16_t read_uint16_be(uint8_t b1, uint8_t b2);
int32_t read_int32_be(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
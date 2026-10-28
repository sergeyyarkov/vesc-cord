#pragma once

#include <stdint.h>

static inline uint16_t read_uint16_be(uint8_t b1, uint8_t b2) {
  return (uint16_t)(((uint16_t)b1 << 8) | b2);
}

static inline int32_t read_int32_be(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
  return (int32_t)(((uint32_t)b1 << 24)  | 
                  ((uint32_t)b2 << 16)   | 
                  ((uint32_t)b3 << 8)    | 
                  ((uint32_t)b4));
}
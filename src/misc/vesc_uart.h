#pragma once

#include <stdint.h>

typedef struct VescTelemetry {
  uint16_t v_in_x10;
  uint16_t temp_mos_x10;
  int32_t amp_hours_x10000;
  int32_t current_motor_x100;
  int32_t current_in_x100;
} VescTelemetry_t;

void VescTelemetry_Update(void);
VescTelemetry_t *VescTelemetry_Get(void);
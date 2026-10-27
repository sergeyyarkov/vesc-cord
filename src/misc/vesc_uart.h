#pragma once

#include <stdint.h>

typedef struct VescTelemetry {
  float v_in;
  float temp_mos;
  float amp_hours;
  int32_t current_motor;
  int32_t current_in;
} VescTelemetry_t;

void VescTelemetry_Update(void);
VescTelemetry_t *VescTelemetry_Get(void);
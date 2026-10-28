#include "display.h"
#include <stdio.h>
#include "drivers/display/lcd1602.h"
#include "misc/vesc_uart.h"

void Display_Refresh(void) {
  VescTelemetry_t* telemetry = VescTelemetry_Get();

  char vin_out_int_str[8];
  char vin_out_frac_str[8];
  uint16_t vin_int = (uint16_t)telemetry->v_in;
  uint16_t vin_frac = ((uint16_t)(telemetry->v_in * 10.0f)) % 10;

  char amp_hours_out_int_str[8];
  char amp_hours_out_frac_str[8];
  uint16_t amp_hours_int = (uint16_t)telemetry->amp_hours;
  uint16_t amp_hours_frac = ((uint16_t)(telemetry->amp_hours * 10000.0f)) % 10000;

  snprintf(vin_out_int_str, sizeof(vin_out_int_str), "%d", vin_int);
  snprintf(vin_out_frac_str, sizeof(vin_out_frac_str), "%d", vin_frac);

  snprintf(amp_hours_out_int_str, sizeof(amp_hours_out_int_str), "%d", amp_hours_int);
  snprintf(amp_hours_out_frac_str, sizeof(amp_hours_out_frac_str), "%d", amp_hours_frac);

  LCD1602_SetCursor(0, 0);
  LCD1602_Print("Vin:");
  LCD1602_Print(vin_out_int_str);
  LCD1602_Print(".");
  LCD1602_Print(vin_out_frac_str);

  LCD1602_SetCursor(0, 1);
  LCD1602_Print("AmpH:");
  LCD1602_Print(amp_hours_out_int_str);
  LCD1602_Print(".");
  LCD1602_Print(amp_hours_out_frac_str);
}
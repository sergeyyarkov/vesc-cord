#include "display.h"
#include <stdio.h>
#include "drivers/display/lcd1602.h"
#include "hal/sys_tick.h"
#include "misc/vesc_uart.h"

void Display_Refresh(void) {
  VescTelemetry_t* telemetry = VescTelemetry_Get();

  /**
   * Battery voltage 
   */
  LCD1602_SetCursor(0, 0);
  LCD1602_Print("Vin:");
  LCD1602_PrintFixed(telemetry->v_in_x10, 10);

  /**
   * Battery current
   */
  LCD1602_SetCursor(9, 0);
  LCD1602_Print("Ib:");
  LCD1602_PrintFixed(telemetry->current_in_x100, 100);

  /**
   * Amp hours
   */
  LCD1602_SetCursor(0, 1);
  LCD1602_Print("Ahr:");
  LCD1602_PrintFixed(telemetry->amp_hours_x10000, 10000);
}

void Display_Init(void) {
  LCD1602_Init();
  LCD1602_Clear();
  LCD1602_Print("vesc_cord\nstartup");
  SysTick_DelayMS(2000);
  LCD1602_Clear();
}
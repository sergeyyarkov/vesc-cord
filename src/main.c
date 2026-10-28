#include <stdbool.h>
#include <stdint.h>
#include "../platform/ch32v00x/system_ch32v00x.h"
#include "app/display.h"
#include "drivers/display/lcd1602.h"
#include "hal/spi.h"
#include "hal/usart.h"
#include "hal/led.h"
#include "misc/scheduler.h"
#include "misc/vesc_uart.h"


int main(void) {
  SystemInit();

  USART_Configure();
  SPI_Configure();
  LED_LiveBitConfigure();
  Scheduler_Init();

  Scheduler_CreateTask(Display_Refresh, 500);
  Scheduler_CreateTask(LED_LiveBitRun, 300);
  Scheduler_CreateTask(VescTelemetry_Update, 50);
  // Scheduler_CreateTask(ADC_Conversion, 1);

  LCD1602_Init();
  LCD1602_Clear();
  
  System_Delay_MS(100);
  while (1) {
    Scheduler_Process();
  }
}
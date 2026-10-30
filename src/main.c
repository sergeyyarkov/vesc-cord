#include <stdbool.h>
#include <stdint.h>
#include "../platform/ch32v00x/system_ch32v00x.h"
#include "app/display.h"
// #include "app/throttle.h"
#include "hal/adc.h"
#include "hal/spi.h"
#include "hal/sys_tick.h"
#include "hal/usart.h"
#include "hal/led.h"
#include "misc/scheduler.h"
#include "misc/vesc_uart.h"


int main(void) {
  SystemInit();
  
  SysTick_Configure();
  ADC_Configure();
  USART_Configure();
  SPI_Configure();
  LED_LiveBitConfigure();
  Display_Init();

  Scheduler_CreateTask(Display_Refresh, 500);
  Scheduler_CreateTask(LED_LiveBitRun, 300);
  Scheduler_CreateTask(VescTelemetry_Update, 50);
  // Scheduler_CreateTask(Throttle_Polling, 5);
  Scheduler_CreateTask(ADC_Conversion, 1);

  while (1) {
    Scheduler_Process();
  }
}
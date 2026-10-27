#include "../platform/ch32v00x/system_ch32v00x.h"
#include "debug.h"
// #include "hal/adc.h"
#include "hal/usart.h"
#include "hal/led.h"
#include "misc/scheduler.h"
#include "misc/vesc_uart.h"

int main(void) {
  SystemInit();
  Delay_Init();

  USART_Configure();
  LED_LiveBitConfigure();
  Scheduler_Init();

  Scheduler_CreateTask(LED_LiveBitRun, 300);
  Scheduler_CreateTask(VescTelemetry_Update, 50);
  // Scheduler_CreateTask(ADC_Conversion, 1);

  while (1) {
    Scheduler_Process();
  }
}
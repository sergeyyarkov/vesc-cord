#include <stddef.h>
#include <ch32v00x.h>
#include "system_ch32v00x.h"
#include "scheduler.h"

#define TASK_MAX_SIZE 16
#define TASK_PARAM_IS_ONCE_FLAG 0
#define TASK_PARAM_IS_DISABLED_FLAG 1

volatile uint32_t sys_time_ms = 0;

__attribute__((interrupt("WCH-Interrupt-fast"))) void SysTick_Handler(void);

Task_t tasks[TASK_MAX_SIZE];

void SysTick_Handler(void) {
  sys_time_ms++;
  SysTick->SR = 0;
}

void Scheduler_Init(void) {
  SysTick->SR = 0; // Clear status register
  SysTick->CNT = 0; // Clear counter register
  
  NVIC_SetPriority(SysTicK_IRQn, 0x00); // High priority
  NVIC_EnableIRQ(SysTicK_IRQn);

  SysTick->CMP = (SystemCoreClock / 1000) - 1; // 1ms

  /**
   * bit 0 - Start the system counter STK
   * bit 1 - Enabling counter interrupts
   * bit 2 - HCLK for time base
   * bit 3 - Count up to the comparison value and start counting from 0 again
   */
  SysTick->CTLR |= (1<<0) | (1<<1) | (1<<2) | (1<<3);
}

// todo: add Scheduler_SetImmediate to run tasks without system timer check

Task_t *Scheduler_CreateTask(void (*handler)(void), uint32_t interval_ms) {
  for (uint8_t i = 0; i < TASK_MAX_SIZE; i++) {
    if (tasks[i].handler == NULL) {
      tasks[i].ms = sys_time_ms;
      tasks[i].interval_ms = interval_ms;
      tasks[i].handler = handler;
      tasks[i].flags = 0x00;

      return &tasks[i];
    }
  }

  return NULL;
}

Task_t *Scheduler_CreateTaskOnce(void (*handler)(void), uint32_t interval_ms) {
  Task_t *task = Scheduler_CreateTask(handler, interval_ms);

  if (task != NULL) {
    task->flags |= (1 << TASK_PARAM_IS_ONCE_FLAG);
  }

  return task;
}

void Scheduler_Process(void) {
  uint32_t current_ms = 0;

  current_ms = sys_time_ms;

  for (uint8_t i = 0; i < TASK_MAX_SIZE; i++) {
    if (tasks[i].handler != NULL) {
      if (tasks[i].flags & (1 << TASK_PARAM_IS_DISABLED_FLAG)) continue;  // skip disabled task

      if (current_ms - tasks[i].ms >= tasks[i].interval_ms) {
        tasks[i].ms = current_ms;
        tasks[i].handler();

        if (tasks[i].flags & (1 << TASK_PARAM_IS_ONCE_FLAG)) {
          tasks[i].handler = NULL;
        }
      }
    }
  }
}

inline void Scheduler_DisableTask(Task_t *task) { task->flags |= (1 << TASK_PARAM_IS_DISABLED_FLAG); }

inline void Scheduler_EnableTask(Task_t *task) { task->flags &= ~(1 << TASK_PARAM_IS_DISABLED_FLAG); }

uint32_t Scheduler_GetSysTimeMs(void) {
  uint32_t time_ms = 0;

  time_ms = sys_time_ms;

  return time_ms;
}

void System_Delay_MS(uint32_t ms) {
  uint32_t start = Scheduler_GetSysTimeMs();
  while ((Scheduler_GetSysTimeMs() - start) < ms);
}
#pragma once

#include <stdint.h>

typedef struct {
  uint32_t ms;
  uint32_t interval_ms;
  void (*handler)(void);
  uint8_t flags;
} Task_t;

void Scheduler_Init(void);
Task_t *Scheduler_CreateTask(void (*handler)(void), uint32_t interval_ms);
Task_t *Scheduler_CreateTaskOnce(void (*handler)(void), uint32_t interval_ms);
void Scheduler_DisableTask(Task_t *task);
void Scheduler_EnableTask(Task_t *task);
void Scheduler_Process(void);
uint32_t Scheduler_GetSysTimeMs(void);
void System_Delay_MS(uint32_t ms);
#include <stddef.h>
#include "scheduler.h"
#include "hal/sys_tick.h"

#define TASK_MAX_SIZE 16
#define TASK_PARAM_IS_ONCE_FLAG 0
#define TASK_PARAM_IS_DISABLED_FLAG 1

Task_t tasks[TASK_MAX_SIZE];

// todo: add Scheduler_SetImmediate to run tasks without system timer check

Task_t *Scheduler_CreateTask(void (*handler)(void), uint32_t interval_ms) {
  for (uint8_t i = 0; i < TASK_MAX_SIZE; i++) {
    if (tasks[i].handler == NULL) {
      tasks[i].ms = SysTick_GetSysTimeMS();
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

  current_ms = SysTick_GetSysTimeMS();

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
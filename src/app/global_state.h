#pragma once

#include <stdint.h>
// #include "misc/scheduler.h"

typedef struct {
  struct {
    uint16_t rpm;
    uint8_t kmh;
  } motor;

  /**
   * Program tasks
   */
  // Task_t *Task_ThrottleMonitor;
} GlobalState_t;
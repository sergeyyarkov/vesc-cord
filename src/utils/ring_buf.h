#pragma once

#include <stdbool.h>
#include <stdint.h>

#define RING_BUF_MAX_SIZE 256

typedef struct RingBuffer {
  volatile uint8_t wr_idx;
  volatile uint8_t rd_idx;
  volatile uint8_t data[RING_BUF_MAX_SIZE];
} RingBuffer_t;

bool RingBuffer_IsFull(RingBuffer_t *buf);
bool RingBuffer_IsEmpty(RingBuffer_t *buf);
bool RingBuffer_Write(RingBuffer_t *buf, uint8_t byte);
uint8_t RingBuffer_Read(RingBuffer_t *buf);
volatile uint8_t *RingBuffer_Get(RingBuffer_t *buf);
#include "ring_buf.h"

bool RingBuffer_IsFull(RingBuffer_t *buf) {
  if ((buf->wr_idx + 1) % RING_BUF_MAX_SIZE == buf->rd_idx) return true;

  return false;
}

bool RingBuffer_IsEmpty(RingBuffer_t *buf) { return buf->rd_idx == buf->wr_idx; }

bool RingBuffer_Write(RingBuffer_t *buf, uint8_t byte) {
  if (RingBuffer_IsFull(buf)) return false;

  buf->data[buf->wr_idx] = byte;
  buf->wr_idx = (buf->wr_idx + 1) % RING_BUF_MAX_SIZE;

  return true;
}

uint8_t RingBuffer_Read(RingBuffer_t *buf) {
  uint8_t data;

  if (RingBuffer_IsEmpty(buf)) return 0;

  data = buf->data[buf->rd_idx];
  buf->rd_idx = (buf->rd_idx + 1) % RING_BUF_MAX_SIZE;

  return data;
}

volatile uint8_t *RingBuffer_Get(RingBuffer_t *buf) { return buf->data; }
#pragma once

#include "utils/ring_buf.h"

void USART_Configure(void);
RingBuffer_t *USART_GetRxBuffer(void);
bool USART_IsRxBufferAvailable(void);
void USART_SendChar(char c);
void USART_SendStr(char *str);
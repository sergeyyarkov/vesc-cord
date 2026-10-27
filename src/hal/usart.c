#include "usart.h"
#include "ch32v00x_gpio.h"
#include "ch32v00x_rcc.h"
#include "ch32v00x_usart.h"

RingBuffer_t RX_BUF = {.data = {0}};

void USART_Configure() {
  GPIO_InitTypeDef GPIOD_InitStructure = {0};
  USART_InitTypeDef USART_InitStructure = {0};
  NVIC_InitTypeDef  NVIC_InitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

  /* RX Pin configuration */
  GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIOD_InitStructure);

  /* TX Pin configuration */
  GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOD, &GPIOD_InitStructure);
  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init(USART1, &USART_InitStructure);

  /* Enable interrupt on receive data  */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(USART1, ENABLE);
}

void USART_SendChar(char c) {
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {};
  USART_SendData(USART1, c);
}

void USART_SendStr(char *str) {
  char c;

  while ((c = *str++) != '\0') {
    USART_SendChar(c);
  }
}

RingBuffer_t *USART_GetRxBuffer(void) { return &RX_BUF; }

bool USART_IsRxBufferAvailable(void) { return !(RingBuffer_IsEmpty(&RX_BUF)); }

void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void USART1_IRQHandler(void) {
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
    RingBuffer_Write(&RX_BUF, USART_ReceiveData(USART1));
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}
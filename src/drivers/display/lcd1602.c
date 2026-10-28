#include <stdint.h>
#include "ch32v00x.h"
#include "hal/spi.h"
#include "misc/scheduler.h"
#include "lcd1602.h"

#define LCD1602_RS_MASK   (1 << 0)
#define LCD1602_RW_MASK   (1 << 1)
#define LCD1602_EN_MASK   (1 << 2)

void LCD1602_Delay_Short(void) {
  for (uint8_t i = 0; i < 200; i++) {
    __NOP();
  }
}

void LCD1602_WriteNibble(uint8_t n) {
  SPI_SendByte(n | LCD1602_EN_MASK);
  LCD1602_Delay_Short();
  SPI_SendByte(n & ~LCD1602_EN_MASK);
  LCD1602_Delay_Short();
}

void LCD1602_WriteByte(uint8_t byte, bool reg_sel) {
  uint8_t high_n = byte & 0xF0;
  uint8_t low_n = (byte << 4) & 0xF0;
  uint8_t rs_bit = reg_sel ? LCD1602_RS_MASK : 0;

  SPI_SendByte(high_n | rs_bit | LCD1602_EN_MASK);
  LCD1602_Delay_Short();
  SPI_SendByte((high_n | rs_bit) & ~LCD1602_EN_MASK);
  LCD1602_Delay_Short();

  SPI_SendByte(low_n | rs_bit | LCD1602_EN_MASK);
  LCD1602_Delay_Short();
  SPI_SendByte((low_n | rs_bit) & ~LCD1602_EN_MASK);
  LCD1602_Delay_Short();
}

void LCD1602_Print(char *str) {
  while (*str) {
    LCD1602_WriteByte(*str++, 1);
  }
}

void LCD1602_SetCursor(uint8_t col, uint8_t row) {
  uint8_t cmd = 0x80;

  if (row > 1) row = 1;
  if (col > 15) col = 15;
  
  if (row > 0) col += 0x40;
  
  cmd |= col;

  LCD1602_WriteByte(cmd, 0);
  LCD1602_Delay_Short();
}

void LCD1602_Clear(void) {
  LCD1602_WriteByte(0x01, 0);
  System_Delay_MS(10);
}

void LCD1602_Init(void){
  uint8_t cmd = 0x30;

  SPI_SendByte(0x00);

  System_Delay_MS(50);

  LCD1602_WriteNibble(cmd);
  System_Delay_MS(5);

  LCD1602_WriteNibble(cmd);
  System_Delay_MS(1);
  LCD1602_WriteNibble(cmd);
  System_Delay_MS(1);
  cmd = 0x20;
  LCD1602_WriteNibble(cmd);
  System_Delay_MS(1);

  cmd = 0x28; // 4-bit, 2 lines, 5x8 dot font

  LCD1602_WriteByte(cmd, 0);
  System_Delay_MS(1);

  cmd = 0x0c; // Set Display, cursor off, bliking off
  LCD1602_WriteByte(cmd, 0);
  System_Delay_MS(1);

  cmd = 0x06; // Increment cursor automatically
  LCD1602_WriteByte(cmd, 0);
  System_Delay_MS(1);

  cmd = 0x01;
  LCD1602_WriteByte(cmd, 0);
  System_Delay_MS(10);

  LCD1602_SetCursor(0, 1);
}
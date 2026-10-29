#include <stdint.h>
#include <sys/_intsup.h>
#include <sys/_types.h>
#include "lcd1602.h"
#include "hal/spi.h"
#include "hal/sys_tick.h"
#include "utils/itoa.h"

#define RS_MASK   (1 << 0)
#define RW_MASK   (1 << 1)
#define EN_MASK   (1 << 2)
#define BL_MASK   (1 << 3)

#define CMD_CLR_DISPLAY       0x01
#define CMD_RET_HOME          0x02
#define CMD_SET_DDRAM         0x80
#define CMD_FUNSET_OPT_1      0x28 // 4-bit, 2 lines, 5x8 dot font
#define CMD_DISP_OFF          0x08 
#define CMD_DISP_ON           0x0C 
#define CMD_DISP_ON_CUR       0x0E 
#define CMD_DISP_ON_BLINK     0x0F 
#define CMD_ENTRY_MODE        0x04 
#define CMD_ENTRY_RIGHT       0x06 
#define CMD_ENTRY_LEFT        0x04 
#define CMD_ENTRY_SH_LEFT     0x07 
#define CMD_ENTRY_SH_RIGHT    0x05 

static inline void Delay_Short(void) { SysTick_DelayUS(3); }
static inline void Delay_MS(uint32_t ms) { SysTick_DelayMS(ms); }

void LCD1602_SetBacklight(bool enable) {
  enable ? SPI_SendByte(0x00 | BL_MASK) : SPI_SendByte(0x00 & ~BL_MASK);
}

void LCD1602_WriteNibble(uint8_t n) {
  SPI_SendByte(n | EN_MASK);
  Delay_Short();
  SPI_SendByte(n & ~EN_MASK);
  Delay_Short();
}

void LCD1602_WriteByte(uint8_t byte, bool reg_sel) {
  uint8_t high_n = byte & 0xF0;
  uint8_t low_n = (byte << 4) & 0xF0;
  uint8_t rs_bit = reg_sel ? RS_MASK : 0;

  SPI_SendByte(high_n | rs_bit | EN_MASK);
  Delay_Short();
  SPI_SendByte((high_n | rs_bit) & ~EN_MASK);
  Delay_Short();

  SPI_SendByte(low_n | rs_bit | EN_MASK);
  Delay_Short();
  SPI_SendByte((low_n | rs_bit) & ~EN_MASK);
  Delay_Short();
}

void LCD1602_Print(char *str) {
  while (*str) {
    if (*str == '\r' || *str == '\n') {
      str++;
      while (*str == '\r' || *str == '\n') str++;
      LCD1602_SetCursor(0, 1);
      continue;
    }
    LCD1602_WriteByte(*str++, 1);
  }
}


void LCD1602_PrintInt(int num) {
  char output[32] = {0};
  util_itoa(num, output);
  LCD1602_Print(output);
}

void LCD1602_SetCursor(uint8_t col, uint8_t row) {
  uint8_t cmd = 0x80; // set ddram function command

  if (row > 1) row = 1;
  if (col > 15) col = 15;
  
  if (row > 0) col += 0x40; // start from second row
  
  cmd |= col;

  LCD1602_WriteByte(cmd, 0);
  Delay_Short();
}

void LCD1602_Clear(void) {
  LCD1602_WriteByte(0x01, 0);
  Delay_MS(10);
}

void LCD1602_PrintFixed(int num, unsigned int scale) {
  if (num < 0) {
    LCD1602_Print("-");
    num = -num;
  }

  unsigned int int_part = (unsigned int)num / scale;

  LCD1602_PrintInt(int_part);
  LCD1602_Print(".");

  unsigned int tmp, tmp2 = num, tmp3 = scale;

  while (scale > 1) {
    tmp = num % 10;
    if (tmp == 0) LCD1602_Print("0");
    num /= 10;
    scale /= 10;
  }

  LCD1602_PrintInt(tmp2 % tmp3);
}

void LCD1602_Init(void){
  SPI_SendByte(0x00); // clear all outputs first

  Delay_MS(50);

  LCD1602_WriteNibble(0x30);
  Delay_MS(5);

  LCD1602_WriteNibble(0x30);
  Delay_MS(1);
  LCD1602_WriteNibble(0x30);
  Delay_MS(1);
  LCD1602_WriteNibble(0x20); // set 4-bit mode
  Delay_MS(1);

  LCD1602_WriteByte(CMD_FUNSET_OPT_1, 0);
  Delay_MS(1);

  LCD1602_WriteByte(CMD_DISP_ON, 0);
  Delay_MS(1);

  LCD1602_WriteByte(CMD_ENTRY_RIGHT, 0);
  Delay_MS(1);

  LCD1602_WriteByte(CMD_CLR_DISPLAY, 0);
  Delay_MS(10);

  LCD1602_SetCursor(0, 0);
}
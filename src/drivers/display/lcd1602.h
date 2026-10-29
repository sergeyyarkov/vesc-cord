#pragma once

#include <stdbool.h>
#include <stdint.h>

void LCD1602_Init(void);
void LCD1602_WriteByte(uint8_t byte, bool reg_sel);
void LCD1602_WriteNibble(uint8_t n);
void LCD1602_Print(char *str);
void LCD1602_PrintInt(int num);
void LCD1602_PrintFixed(int num, unsigned int scale);
void LCD1602_Clear(void);
void LCD1602_SetCursor(uint8_t col, uint8_t row);
void LCD1602_SetBacklight(bool enable);
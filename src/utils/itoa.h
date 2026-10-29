#pragma once

#include <stdbool.h>

static inline char* util_itoa(int num, char *str) {
  unsigned int i = 0, j = 0, tmp = 0;
  bool is_neg = num < 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }
  
  if (num <= 9 && !is_neg) {
    str[i++] = num + '0';
    str[i] = '\0';
    return str;
  }

  unsigned int abs_num;

  if (is_neg) {
    str[i] = '-';
    abs_num = -(unsigned int)num;
    i++;
    j = 1;
  } else {
    abs_num = (unsigned int)num;
  }

  while (abs_num) {
    str[i++] = (abs_num % 10) + '0';
    abs_num /= 10;
  } 

  str[i] = '\0';

  i -= 1; // skip \0

  while (j < i) {
    tmp = str[j];
    str[j++] = str[i];
    str[i--] = tmp;
  }

  return str;
}
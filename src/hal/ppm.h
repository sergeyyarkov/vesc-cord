#pragma once

#include <stdint.h>

void PPM_Configure(void);

/**
 * @brief Set duty cycle of servo signal
 *
 * @param value - from 0% to 100%
 */
void PPM_SetDutyCycle(uint8_t value);
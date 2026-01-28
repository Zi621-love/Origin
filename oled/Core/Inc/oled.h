/*
 * oled.h
 *
 *  Created on: Jan 28, 2026
 *      Author: 33479
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "i2c.h"

void OLED_Test();

void OLED_Init();

void OLED_ShowFrame();

void OLED_NewFrame();

void OLED_SetPixel(uint8_t x, uint8_t y);

#endif /* INC_OLED_H_ */

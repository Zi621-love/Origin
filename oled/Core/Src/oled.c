#include "oled.h"
#include <string.h>

#define OLED_ADDRESS 0x07A

void OLED_SendCmd(uint8_t cmd){
	uint8_t sendBuffer[2];
	sendBuffer[0] = 0x00;
	sendBuffer[1] = cmd;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuffer, 2, HAL_MAX_DELAY);
}

void OLED_Init(){

}

uint8_t GRAM[8][128];

void OLED_NewFrame(){
//	for(uint8_t i = 0; i < 8; i++){
//		for (uint8_t j = 0; j < 128; j++){
//			GRAM[i][j] = 0;
//		}
//	}
	memset(GRAM, 0, sizeof(GRAM));
}

void OLED_ShowFrame(){
	uint8_t sendBuffer[129];
	sendBuffer[0] = 0x40;
	for(uint8_t i = 0; i < 8; i++){
		for(uint8_t j = 0; j < 128; j++){
			sendBuffer[j+1] = GRAM[i][j];
		}
		OLED_SendCmd(0xB0 + i);
		OLED_SendCmd(0x02);
		OLED_SendCmd(0x10);
		HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuffer, sizeof(sendBuffer), HAL_MAX_DELAY);
	}
}

void OLED_SetPixel(uint8_t x, uint8_t y){
	if (x >= 128 || y > 64) return;
	GRAM[y/8][x] |= 0x01 << (y % 8);
}

void OLED_Test(){
	OLED_SendCmd(0xB0);
	OLED_SendCmd(0x02);
	OLED_SendCmd(0x10);

	uint8_t sendBuffer[] = { 0x40, 0xAA};
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuffer, sizeof(sendBuffer), HAL_MAX_DELAY);
}

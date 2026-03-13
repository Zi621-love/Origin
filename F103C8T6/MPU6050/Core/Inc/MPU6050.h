#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32f1xx_hal.h"

#define MPU6050_ADDR (0x68 << 1)  // I2C设备地址（左移1位）

// MPU6050寄存器地址
#define PWR_MGMT_1     0x6B
#define ACCEL_CONFIG   0x1C
#define GYRO_CONFIG    0x1B
#define ACCEL_XOUT_H   0x3B
#define GYRO_XOUT_H    0x43

// 原始数据结构体
typedef struct {
    int16_t Accel_X, Accel_Y, Accel_Z;
    int16_t Gyro_X, Gyro_Y, Gyro_Z;
} MPU6050_Data;

// 函数声明
uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c);
uint8_t MPU6050_ReadRawData(I2C_HandleTypeDef *hi2c, MPU6050_Data *data);
void MPU6050_ConvertToPhysical(MPU6050_Data *raw, float *accel, float *gyro);

#endif

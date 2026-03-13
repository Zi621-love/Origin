#include "mpu6050.h"
#include <math.h>

// 写入寄存器
static uint8_t MPU6050_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value) {
    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
}

// 初始化MPU6050
uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    if (HAL_I2C_IsDeviceReady(hi2c, MPU6050_ADDR, 3, 100) != HAL_OK) {
        return HAL_ERROR;  // 设备未连接
    }
    if (MPU6050_WriteReg(hi2c, PWR_MGMT_1, 0x01) != HAL_OK) return HAL_ERROR;  // 唤醒设备
    HAL_Delay(100);
    if (MPU6050_WriteReg(hi2c, ACCEL_CONFIG, 0x00) != HAL_OK) return HAL_ERROR;  // 加速度计±2g
    if (MPU6050_WriteReg(hi2c, GYRO_CONFIG, 0x00) != HAL_OK) return HAL_ERROR;   // 陀螺仪±250°/s
    return HAL_OK;
}

// 读取原始数据（14字节：加速度6字节+温度2字节+陀螺仪6字节）
uint8_t MPU6050_ReadRawData(I2C_HandleTypeDef *hi2c, MPU6050_Data *data) {
    uint8_t buf[14];
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, buf, 14, 100) != HAL_OK) {
        return HAL_ERROR;
    }
    // 合并高低位字节（大端模式）
    data->Accel_X = (int16_t)(buf[0] << 8 | buf[1]);
    data->Accel_Y = (int16_t)(buf[2] << 8 | buf[3]);
    data->Accel_Z = (int16_t)(buf[4] << 8 | buf[5]);
    data->Gyro_X  = (int16_t)(buf[8] << 8 | buf[9]);
    data->Gyro_Y  = (int16_t)(buf[10] << 8 | buf[11]);
    data->Gyro_Z  = (int16_t)(buf[12] << 8 | buf[13]);  // 注意：索引12和13应为陀螺仪Z[1](@ref)
    return HAL_OK;
}

// 将原始数据转换为物理值（单位：m/s² 和 rad/s）
void MPU6050_ConvertToPhysical(MPU6050_Data *raw, float *accel, float *gyro) {
    const float accel_scale = 16384.0;  // ±2g量程下的灵敏度
    const float gyro_scale = 131.0;     // ±250°/s量程下的灵敏度
    const float pi = 3.14159265359f;

    // 转换加速度：原始值 -> g -> m/s²
    accel[0] = (raw->Accel_X / accel_scale) * 9.8f;  // X轴
    accel[1] = (raw->Accel_Y / accel_scale) * 9.8f;  // Y轴
    accel[2] = (raw->Accel_Z / accel_scale) * 9.8f;  // Z轴

    // 转换角速度：原始值 -> °/s -> rad/s
    gyro[0] = (raw->Gyro_X / gyro_scale) * (pi / 180.0f);  // X轴
    gyro[1] = (raw->Gyro_Y / gyro_scale) * (pi / 180.0f);  // Y轴
    gyro[2] = (raw->Gyro_Z / gyro_scale) * (pi / 180.0f);  // Z轴
}


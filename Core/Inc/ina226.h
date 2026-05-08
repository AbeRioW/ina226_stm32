#ifndef __INA226_H
#define __INA226_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define INA226_DEFAULT_ADDR  0x40 << 1

#define INA226_REG_CONFIG    0x00
#define INA226_REG_SHUNT_V   0x01
#define INA226_REG_BUS_V     0x02
#define INA226_REG_POWER     0x03
#define INA226_REG_CURRENT   0x04
#define INA226_REG_CAL       0x05
#define INA226_REG_MASK      0x06
#define INA226_REG_ALERT     0x07
#define INA226_REG_MFG_ID    0xFE
#define INA226_REG_DIE_ID    0xFF

#define INA226_MFG_ID_VALUE  0x5449
#define INA226_DIE_ID_VALUE  0x2260

typedef struct {
    I2C_HandleTypeDef *i2c;
    uint8_t addr;
    float shunt_resistor;
    float current_lsb;
    uint16_t cal_value;
} INA226_HandleTypeDef;

HAL_StatusTypeDef INA226_CheckDevice(INA226_HandleTypeDef *ina);
HAL_StatusTypeDef INA226_Init(INA226_HandleTypeDef *ina, I2C_HandleTypeDef *hi2c, uint8_t addr);
HAL_StatusTypeDef INA226_SetCalibration(INA226_HandleTypeDef *ina, float max_current, float shunt_resistor);
float INA226_GetBusVoltage(INA226_HandleTypeDef *ina);
float INA226_GetShuntVoltage(INA226_HandleTypeDef *ina);
float INA226_GetCurrent(INA226_HandleTypeDef *ina);
float INA226_GetPower(INA226_HandleTypeDef *ina);
uint16_t INA226_ReadRegister(INA226_HandleTypeDef *ina, uint8_t reg);
HAL_StatusTypeDef INA226_WriteRegister(INA226_HandleTypeDef *ina, uint8_t reg, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif

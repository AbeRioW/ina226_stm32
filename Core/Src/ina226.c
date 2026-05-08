#include "ina226.h"

HAL_StatusTypeDef INA226_CheckDevice(INA226_HandleTypeDef *ina)
{
    uint16_t mfg_id = INA226_ReadRegister(ina, INA226_REG_MFG_ID);
    uint16_t die_id = INA226_ReadRegister(ina, INA226_REG_DIE_ID);
    
    if (mfg_id == INA226_MFG_ID_VALUE && (die_id & 0xFFF0) == INA226_DIE_ID_VALUE) {
        return HAL_OK;
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef INA226_Init(INA226_HandleTypeDef *ina, I2C_HandleTypeDef *hi2c, uint8_t addr)
{
    ina->i2c = hi2c;
    ina->addr = addr;
    ina->shunt_resistor = 0.01f;
    ina->current_lsb = 0.001f;
    
	    if (INA226_CheckDevice(ina) != HAL_OK) {
        return HAL_ERROR;
    }
	
    return INA226_WriteRegister(ina, INA226_REG_CONFIG, 0x4127);
}

HAL_StatusTypeDef INA226_SetCalibration(INA226_HandleTypeDef *ina, float max_current, float shunt_resistor)
{
    ina->shunt_resistor = shunt_resistor;
    ina->current_lsb = max_current / 32768.0f;
    
    ina->cal_value = (uint16_t)(0.00512f / (ina->current_lsb * ina->shunt_resistor));
    
    return INA226_WriteRegister(ina, INA226_REG_CAL, ina->cal_value);
}

uint16_t INA226_ReadRegister(INA226_HandleTypeDef *ina, uint8_t reg)
{
    uint8_t data[2];
    
    if (HAL_I2C_Mem_Read(ina->i2c, ina->addr, reg, 1, data, 2, HAL_MAX_DELAY) != HAL_OK) {
        return 0;
    }
    
    return (data[0] << 8) | data[1];
}

HAL_StatusTypeDef INA226_WriteRegister(INA226_HandleTypeDef *ina, uint8_t reg, uint16_t value)
{
    uint8_t data[2];
    data[0] = (value >> 8) & 0xFF;
    data[1] = value & 0xFF;
    
    return HAL_I2C_Mem_Write(ina->i2c, ina->addr, reg, 1, data, 2, HAL_MAX_DELAY);
}

float INA226_GetBusVoltage(INA226_HandleTypeDef *ina)
{
    uint16_t raw = INA226_ReadRegister(ina, INA226_REG_BUS_V);
    return raw * 0.00125f;
}

float INA226_GetShuntVoltage(INA226_HandleTypeDef *ina)
{
    int16_t raw = (int16_t)INA226_ReadRegister(ina, INA226_REG_SHUNT_V);
    return raw * 0.0000025f;
}

float INA226_GetCurrent(INA226_HandleTypeDef *ina)
{
    int16_t raw = (int16_t)INA226_ReadRegister(ina, INA226_REG_CURRENT);
    return raw * ina->current_lsb;
}

float INA226_GetPower(INA226_HandleTypeDef *ina)
{
    uint16_t raw = INA226_ReadRegister(ina, INA226_REG_POWER);
    return raw * ina->current_lsb * 25.0f;
}

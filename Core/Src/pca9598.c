/*
 * pca9598.c
 *
 *  Created on: 9 нояб. 2022 г.
 *      Author: come_ill_foo
 */

#include "pca9598.h"
#include "i2c.h"

HAL_StatusTypeDef PCA9538_Read_Register(uint16_t addr, pca9538_regs_t reg, uint8_t* buf) {
	return HAL_I2C_Mem_Read(&hi2c1, addr | 1, reg, 1, buf, 1, 100);
}


HAL_StatusTypeDef PCA9538_Write_Register(uint16_t addr, pca9538_regs_t reg, uint8_t* buf) {
	return HAL_I2C_Mem_Write(&hi2c1, addr & 0xFFFE, reg, 1, buf, 1, 100);
}


HAL_StatusTypeDef PCA9538_Read_Inputs(uint16_t addr, uint8_t* buf) {
	return PCA9538_Read_Register(addr, INPUT_PORT, buf);
}

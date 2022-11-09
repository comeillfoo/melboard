/*
 * pca9598.h
 *
 *  Created on: 9 нояб. 2022 г.
 *      Author: come_ill_foo
 */

#ifndef INC_PCA9598_H_
#define INC_PCA9598_H_

#include "stm32f4xx_hal.h"

#define DIP_RD_ADDR 0xE1
#define DIP_WR_ADDR 0xE0



typedef enum{
	INPUT_PORT = 0x00, //Read byte XXXX XXXX
	OUTPUT_PORT = 0x01, //Read/write byte 1111 1111
	POLARITY_INVERSION = 0x02, //Read/write byte 0000 0000
	CONFIG = 0x03 //Read/write byte 1111 1111
} pca9538_regs_t;

typedef enum{
	LOW = 0,
	HIGH = 1
} what_bytes_t;

HAL_StatusTypeDef PCA9538_Read_Register(uint16_t addr, pca9538_regs_t reg, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Write_Register(uint16_t addr, pca9538_regs_t reg, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Read_Inputs(uint16_t addr, uint8_t* buf);

#endif /* INC_PCA9598_H_ */

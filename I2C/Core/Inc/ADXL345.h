#ifndef ADXL345_H
#define ADXL345_H

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
// p.14 I2C is enabled if CS is tied to HIGH

// p.17 if SDO/ALT ADDRESS pin is tied LOW the I2C address is 0x53. By default it is tied to LOW
// if SDO/ALT ADDRESS pin is tied LOW the I2C address is 0x1D
#define ADXL345_ADDRESS 			(0x53 << 1) 		// (0x1D << 1) if tied high p.17


#define ADXL345_DEVICE_ID  			0xE5				// p.24


//Registers p.22

#define ADXL345_REG_DEVICE_ID     	0x00	// Implemented
#define ADXL345_REG_THRESH_TAP    	0x1D
#define ADXL345_REG_OFSX          	0x1E 	// Implemented
#define ADXL345_REG_OFSY          	0x1F 	// Implemented
#define ADXL345_REG_OFSZ          	0x20	// Implemented
#define ADXL345_REG_DUR           	0x21
#define ADXL345_REG_LATENT        	0x22
#define ADXL345_REG_WINDOW        	0x23
#define ADXL345_REG_THRESH_ACT    	0x24
#define ADXL345_REG_THRESH_INACT  	0x25
#define ADXL345_REG_TIME_INACT    	0x26
#define ADXL345_REG_ACT_INACT_CTL 	0x27
#define ADXL345_REG_THRESH_FF     	0x28
#define ADXL345_REG_TIME_FF       	0x29
#define ADXL345_REG_TAP_AXES      	0x2A
#define ADXL345_REG_ACT_TAP_STATUS 	0x2B
#define ADXL345_REG_BW_RATE      	0x2C
#define ADXL345_REG_POWER_CTL     	0x2D
#define ADXL345_REG_INT_ENABLE    	0x2E
#define ADXL345_REG_INT_MAP       	0x2F
#define ADXL345_REG_INT_SOURCE    	0x30
#define ADXL345_REG_DATA_FORMAT   	0x31
#define ADXL345_REG_DATAX0        	0x32	// Implemented
#define ADXL345_REG_DATAX1        	0x33	// Implemented
#define ADXL345_REG_DATAY0        	0x34	// Implemented
#define ADXL345_REG_DATAY1        	0x35	// Implemented
#define ADXL345_REG_DATAZ0        	0x36	// Implemented
#define ADXL345_REG_DATAZ1        	0x37	// Implemented
#define ADXL345_REG_FIFO_CTL      	0x38
#define ADXL345_REG_FIFO_STATUS   	0x39

typedef struct
{
	// I2C handle
	I2C_HandleTypeDef *hi2cHandle;

	// Raw acceleration data X Y Z
	int16_t acc_rawX;
	int16_t acc_rawY;
	int16_t acc_rawZ;

	// Acceleration data X Y Z
	float acc_msp2X;
	float acc_msp2Y;
	float acc_msp2Z;
} ADXL345;

uint8_t ADXL345_initialization(I2C_HandleTypeDef *hi2c, ADXL345 *dev);

void ADXL345_set_offset(ADXL345 *dev);

HAL_StatusTypeDef ADXL345_get_acc_raw(ADXL345 *dev);
HAL_StatusTypeDef ADXL345_get_acc_norm(ADXL345 *dev);

void ADXL345_print_raw(ADXL345 *dev, UART_HandleTypeDef *huart);
void ADXL345_print_norm(ADXL345 *dev, UART_HandleTypeDef *huart);

#endif

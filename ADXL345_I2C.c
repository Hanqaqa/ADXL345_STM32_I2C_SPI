#include "ADXL345_I2C.h"

uint8_t ADXL345_initialization(I2C_HandleTypeDef *hi2c, ADXL345 *dev)
{
	dev->hi2cHandle = hi2c;
    uint8_t regData;
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    // Read if the REGISTER_DEVICE_ID answers with the correct data ADXL345_DEVICE_ID 0xE5 p.24
    status = HAL_I2C_Mem_Read(dev->hi2cHandle, ADXL345_ADDRESS, ADXL345_REG_DEVICE_ID, I2C_MEMADD_SIZE_8BIT, &regData, 1, HAL_MAX_DELAY);
    errNum += (status != HAL_OK);
    if (regData != ADXL345_DEVICE_ID)
    {
    	return 255; //
    }

    // Set the register ADXL345_REG_POWER_CTL to enable measurement mode 0b00001000 = 0x08 p.26
    regData = 0x08;
    status = HAL_I2C_Mem_Write(dev->hi2cHandle, ADXL345_ADDRESS, ADXL345_REG_POWER_CTL, I2C_MEMADD_SIZE_8BIT, &regData, 1, HAL_MAX_DELAY);
    errNum += (status != HAL_OK);

    return 0;
}

void ADXL345_set_offset(ADXL345 *dev)
{
	// p.31 Calculating offsets. For example in Y axis with normalised values I was consistently getting a 0.1 offset value
	// So 0.1/0.0156 = 6.4LSB which approximately is 6LSB. Decimal -6 represented in twos complements is 0b11111010
	uint8_t offsetX = 0b00000000;
	uint8_t offsetY = 0b11111010;
	uint8_t offsetZ = 0b11110100;
	HAL_I2C_Mem_Write(dev->hi2cHandle, ADXL345_ADDRESS, ADXL345_REG_OFSX, I2C_MEMADD_SIZE_8BIT, &offsetX, 1, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(dev->hi2cHandle, ADXL345_ADDRESS, ADXL345_REG_OFSY, I2C_MEMADD_SIZE_8BIT, &offsetY, 1, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(dev->hi2cHandle, ADXL345_ADDRESS, ADXL345_REG_OFSZ, I2C_MEMADD_SIZE_8BIT, &offsetZ, 1, HAL_MAX_DELAY);

}

HAL_StatusTypeDef ADXL345_get_acc_raw(ADXL345 *dev)
{
	uint8_t data[6];

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(dev->hi2cHandle, ADXL345_ADDRESS, ADXL345_REG_DATAX0, I2C_MEMADD_SIZE_8BIT, data, 6, HAL_MAX_DELAY);

	dev->acc_rawX = (int16_t)((data[1] << 8) | data[0]);
	dev->acc_rawY = (int16_t)((data[3] << 8) | data[2]);
	dev->acc_rawZ = (int16_t)((data[5] << 8) | data[4]);

	return status;
}

HAL_StatusTypeDef ADXL345_get_acc_norm(ADXL345 *dev)
{
	HAL_StatusTypeDef status = ADXL345_get_acc_raw(dev);

	dev->acc_msp2X = dev->acc_rawX * 0.004;// * 9.81;
	dev->acc_msp2Y = dev->acc_rawY * 0.004;// * 9.81;
	dev->acc_msp2Z = dev->acc_rawZ * 0.004;// * 9.81;

	return status;
}

void ADXL345_print_raw(ADXL345 *dev, UART_HandleTypeDef *huart)
{
	  char bufferX[10];
	  char bufferY[10];
	  char bufferZ[10];

	  const char *msgX = "X=";
	  const char *msgY = "Y=";
	  const char *msgZ = "Z=";
	  const char *space = "\t";
	  const char *rn = "\r\n";

	  snprintf(bufferX, sizeof(bufferX), "%d", dev->acc_rawX);
	  snprintf(bufferY, sizeof(bufferY), "%d", dev->acc_rawY);
	  snprintf(bufferZ, sizeof(bufferZ), "%d", dev->acc_rawZ);

	  HAL_UART_Transmit(huart, (uint8_t *)msgX, strlen(msgX), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)bufferX, strlen(bufferX), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)space, strlen(space), HAL_MAX_DELAY);

	  HAL_UART_Transmit(huart, (uint8_t *)msgY, strlen(msgY), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)bufferY, strlen(bufferY), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)space, strlen(space), HAL_MAX_DELAY);

	  HAL_UART_Transmit(huart, (uint8_t *)msgZ, strlen(msgZ), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)bufferZ, strlen(bufferZ), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)space, strlen(space), HAL_MAX_DELAY);

	  HAL_UART_Transmit(huart, (uint8_t *)rn, strlen(rn), HAL_MAX_DELAY);
}


void ADXL345_print_norm(ADXL345 *dev, UART_HandleTypeDef *huart)
{
	  char bufferX[10];
	  char bufferY[10];
	  char bufferZ[10];

	  const char *msgX = "X=";
	  const char *msgY = "Y=";
	  const char *msgZ = "Z=";
	  const char *space = "\t";
	  const char *rn = "\r\n";

	  snprintf(bufferX, sizeof(bufferX), "%f", dev->acc_msp2X);
	  snprintf(bufferY, sizeof(bufferY), "%f", dev->acc_msp2Y);
	  snprintf(bufferZ, sizeof(bufferZ), "%f", dev->acc_msp2Z);

	  HAL_UART_Transmit(huart, (uint8_t *)msgX, strlen(msgX), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)bufferX, strlen(bufferX), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)space, strlen(space), HAL_MAX_DELAY);

	  HAL_UART_Transmit(huart, (uint8_t *)msgY, strlen(msgY), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)bufferY, strlen(bufferY), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)space, strlen(space), HAL_MAX_DELAY);

	  HAL_UART_Transmit(huart, (uint8_t *)msgZ, strlen(msgZ), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)bufferZ, strlen(bufferZ), HAL_MAX_DELAY);
	  HAL_UART_Transmit(huart, (uint8_t *)space, strlen(space), HAL_MAX_DELAY);

	  HAL_UART_Transmit(huart, (uint8_t *)rn, strlen(rn), HAL_MAX_DELAY);
}


#include "ADXL345_SPI.h"



uint8_t ADXL345_initialization(SPI_HandleTypeDef *hspi, ADXL345 *dev)
{
	dev->hspiHandle = hspi;
    uint8_t regAddr = ADXL345_REG_DEVICE_ID | 0x80; // 0x80 means READ 1 byte
    uint8_t regData;
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    // Read if the REGISTER_DEVICE_ID answers with the correct data ADXL345_DEVICE_ID 0xE5 p.24
    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY);
	status = HAL_SPI_Receive(dev->hspiHandle, &regData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);
    errNum += (status != HAL_OK);
    if (regData != ADXL345_DEVICE_ID)
    {
    	return 255; //
    }

    // Set the register ADXL345_REG_POWER_CTL to enable measurement mode 0b00001000 = 0x08 p.26
    regAddr = ADXL345_REG_POWER_CTL; // No |0x80 means, WRITE 1 byte
    regData = 0x08;
    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY);
	status = HAL_SPI_Transmit(dev->hspiHandle, &regData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	regAddr = ADXL345_REG_POWER_CTL | 0x80;
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY);
	status = HAL_SPI_Receive(dev->hspiHandle, &regData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

    errNum += (status != HAL_OK);

    return 0;
}

// TODO FINISH the rest of the functionalities in SPI

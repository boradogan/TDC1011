#include "stm32f4xx.h"                  // Device header
#include <main.h>

#define CONFIG_0 0x00
#define CONFIG_1 0x01
#define CONFIG_2 0x02
#define CONFIG_3 0x03
#define CONFIG_4 0x04
#define TOF_1 0x05
#define TOF_0 0x06
#define ERROR_FLAGS 0x07
#define TIMEOUT 0x08
#define CLOCK_RATE 0x09

HAL_StatusTypeDef TDC1011_Config(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint8_t size, uint32_t Timeout);
uint8_t ReceiveData(SPI_HandleTypeDef *hspi, uint8_t Register);
HAL_StatusTypeDef SendData(SPI_HandleTypeDef *hspi, uint8_t Register, uint8_t Data);
void CheckRegister(SPI_HandleTypeDef *hspi);

	

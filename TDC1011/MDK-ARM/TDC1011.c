#include "TDC1011.h"

HAL_StatusTypeDef TDC1011_Config(SPI_HandleTypeDef *hspi){
	uint16_t StartTick = HAL_GetTick();
	while(HAL_SPI_GetState(hspi) != HAL_SPI_STATE_READY && ((HAL_GetTick() - StartTick) < 500));
	if((HAL_GetTick() - StartTick) >= 500)
		return HAL_TIMEOUT;
	if(SendData(hspi, CONFIG_0, (0x02 << 5) + 0x05) != HAL_OK)                                           // TX_FREQ_DIV = 2h    NUM_TX = 5h default
		return HAL_ERROR;
	if(SendData(hspi, CONFIG_1, 0x41)!= HAL_OK)                                                          // NUM_RX = 1h
		return HAL_ERROR;
	if(SendData(hspi, CONFIG_2, (0x01 << 7) + (0x00 << 6) + (0x01 << 5))!= HAL_OK)	                     // VCOM_SEL = 1h (External),   MEAS_MODE = 0h (TOF), DAMPING = 1h (Enabled)
		return HAL_ERROR;
	if(SendData(hspi, CONFIG_3, 0x03)!= HAL_OK)                                                          // ECHO_QUAL_THLD =3h (125mV) default
		return HAL_ERROR;
	if(SendData(hspi, CONFIG_4, (0x01 << 6) + (0x00 << 5) + (0x1F))!= HAL_OK)                            // RECEIVE_MODE = 1h (Multi Echo), TRIG_EDGE_POLARITY = 0h (Rising Edge), TX_PH_SHIFT_POS = 1Fh (Position 31)
		return HAL_ERROR;
	if(SendData(hspi, TOF_1, (0x07 << 5) + (0x00 << 4) + (0x00 << 3) + (0x00 << 2) + (0x00))!= HAL_OK)   // PGA_GAIN = 7h (21 dB),  PGA_CTRL = 0h (Active), LNA_CTRL = 0h (Active), LNA_FB = 0h (Capacitive Feedback), TIMING_REG[9:8] = 0h
		return HAL_ERROR;
	if(SendData(hspi, TOF_0, 0xC8)!= HAL_OK)                                                             // TIMING_REG[7:0] = C8h = 200
		return HAL_ERROR;
	if(SendData(hspi, ERROR_FLAGS, 0x03)!= HAL_OK)                                                       // Error flags are resetted
		return HAL_ERROR;
	if(SendData(hspi, TIMEOUT, (0x00 << 6) + (0x03 << 3) + (0x00 << 2) + (0x03))!= HAL_OK)               // FORCE_SHORT_TOF = 0h (disabled), SHORT_TOF_BLANK_PERIOD = 3h (Default), ECHO_TIMEOUT = 0h (enabled), TOF_TIMEOUT_CTRL = 3h (1024* T0)
		return HAL_ERROR;
	if(SendData(hspi, CLOCK_RATE, (0x00 << 2) + 0x00)!= HAL_OK)                                          //CLOCKIN_DIV = 0h (divide by 1 (Default)), AUTOZERO_PERIOD = 0h (64 * T0(Default))
		return HAL_ERROR;
	
	return HAL_OK;
}

HAL_StatusTypeDef SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint8_t size, uint32_t Timeout){
	HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_RESET);            //Pull Slave Select
	HAL_StatusTypeDef HAL_Stat = HAL_SPI_Transmit(hspi, pData, size, Timeout);
	HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_SET);							//Leave Slave Select
	return HAL_Stat;
}

HAL_StatusTypeDef SendData(SPI_HandleTypeDef *hspi, uint8_t Register, uint8_t Data){
	uint8_t TX_Reg[2];
	TX_Reg[0] = 0x40 + Register;     //0x40 for write operation
	TX_Reg[1] = Data;
	return SPI_Transmit(hspi, TX_Reg, 2, 50);
}



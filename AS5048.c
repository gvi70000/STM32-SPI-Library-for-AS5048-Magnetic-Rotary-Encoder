#include "AS5048.h"
#include "spi.h"
#include "gpio.h"
//https://github.com/ZoetropeLabs/AS5048A-Arduino/blob/master/lib/AS5048A/AS5048A.cpp
//https://github.com/MikroElektronika/Click_Magneto_AS4058A/blob/master/library/src/magneto_hal.c
static uint8_t AS5048_Error[4] = {0, 0, 0, 0};
static uint16_t AS5048_CrtVal[4] = {0, 0, 0, 0};
static uint16_t AS5048_OldVal[4] = {0, 0, 0, 0};

static void set_CS(uint8_t senPin, uint8_t state){
	if(state)	{
		switch(senPin){
			case 0:
				SEN_H_Release;
			break;
			case 1:
				SEN_V_Release;
			break;
		}
	}
	else {
		switch(senPin) {
			case 0:
				SEN_H_Select;
			break;
			case 1:
				SEN_V_Select;
			break;
		}
	}
	for(uint8_t i = 0;i < 200; i++);//short delay for CS toogle
}

static uint16_t read(uint8_t sen, uint16_t registerAddress){
	SPI_AS5048;//Set SPI2->CR1-->CPOL Low
	uint8_t data[2] = {0, 0};
	uint8_t cmd[2] = {0, 0};
	//Split the command into two uint8_t
	cmd[1] = registerAddress & 0xFF;
	cmd[0] = ( registerAddress >> 8 ) & 0xFF;
	//printf("data[0] = %d data[1] = %d \n", data[0], data[1]);
	//Send the command
	set_CS(sen, 0);
	HAL_SPI_Transmit(&hspi1, cmd, 2, 1000);
	set_CS(sen, 1);
	//Now read the response
	set_CS(sen, 0);
	HAL_SPI_TransmitReceive(&hspi1, cmd, data, 4, 1000);
	set_CS(sen, 1);
	//printf("data[0] = %d data[1] = %d \n", data[0], data[1]);
	//Check if the error bit is set
	if (data[0] & 0x40) {
		//printf("I have error \n");
		AS5048_Error[sen] = 1;
	}
	else {
		AS5048_Error[sen] = 0;
	}
	SPI_Default;//Set SPI2->CR1-->CPHA High
	//Return the data, stripping the parity and error bits
	return ((( data[0] & 0xFF) << 8) | (data[1] & 0xFF)) & ~0xC000;
}

uint16_t AS5048_getRelativeValue(uint8_t sensor){
	int16_t tmpVal = 0;
	AS5048_OldVal[sensor] = AS5048_CrtVal[sensor];
	AS5048_CrtVal[sensor] = read(sensor,SPI_REG_DATA);
	tmpVal = AS5048_CrtVal[sensor] - AS5048_OldVal[sensor];
	if(tmpVal < 0){
		return AS5048_CrtVal[sensor] + (AS5048_Max - AS5048_OldVal[sensor]);
	} else {
		return tmpVal;
	}
}

uint16_t AS5048_getValue(uint8_t sensor){
	return read(sensor,SPI_REG_DATA);
}

uint16_t AS5048_getMagnitude(uint8_t sen){
	return read(sen, SPI_REG_MAG);//Received data is thrown away: this data comes from the precedent command (unknown)
}
uint16_t AS5048_getDiagnostics(uint8_t sen){
	return read(sen, SPI_REG_AGC);//Received data is thrown away: this data comes from the precedent command (unknown)
}

uint16_t AS5048_getError(uint8_t sen){
	return read(sen, SPI_REG_CLRERR);//Received data is thrown away: this data comes from the precedent command (unknown)
}

uint8_t AS5048_getErrFlag(uint8_t sen){
	return AS5048_Error[sen];
}

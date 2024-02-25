#ifndef __AS5048_H
#define __AS5048_H

#ifdef __cplusplus
extern "C" {
#endif
#include "spi.h"
//Control and Error Registers
//#define SPI_CMD_READ				0x4000 // flag indicating read attempt
#define SPI_CMD_WRITE				0x8000 // flag indicating write attempt
#define SPI_NOP							0x0000//B0000000000000000 No operation dummy information
#define SPI_REG_AGC					0x7ffd // agc register when using SPI 
#define SPI_REG_MAG					0x7ffe // magnitude register when using SPI
#define SPI_REG_DATA				0xffff // data register when using SPI
#define SPI_REG_CLRERR			0x4001 // clear error register when using SPI
#define SPI_REG_ZEROPOS_HI	0x0016 // zero position register high byte
#define SPI_REG_ ZEROPOS_LO	0x0017 // zero position register low byte
#define AS5048_Max					16383
#define AS5048_Magic				(16384 - 31 - 1)	
#define DPU									0.02197265625//Deg per 14bit unit
#define AS_Count						3//4 sensors attached to SPI2 
	
typedef struct {
	uint8_t Id;	
	uint16_t CrtValue;
	uint16_t PrevValue;
	uint8_t Fault; //handle by interrupt

} myAS5048;
	uint16_t AS5048_getRelativeValue(uint8_t sensor);
	uint16_t AS5048_getValue(uint8_t sensor);
	uint16_t AS5048_getMagnitude(uint8_t sen);
	uint16_t AS5048_getDiagnostics(uint8_t sen);
	uint16_t AS5048_getError(uint8_t sen);
	uint8_t AS5048_getErrFlag(uint8_t sen);
	
#ifdef __cplusplus
}
#endif

#endif /* __AS5048_H */

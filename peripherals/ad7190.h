
#ifndef __AD7190_H
#define __AD7190_H
#include "stdint.h"


void AD7190_Restart();

uint8_t AD7190_ReadData(void);

void AD7190_Transmit(uint8_t data);


void AD7190_Write_ModeReg(void);
uint32_t AD7190_Read_Mode_Reg();

void ad7190_CS_HIGH();

void ad7190_CS_LOW();

#endif


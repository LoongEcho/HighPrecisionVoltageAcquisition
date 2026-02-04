
#ifndef __AD7190_H
#define __AD7190_H
#include "stdint.h"


void AD7190_Restart();
void ad7190_CS_HIGH();
void ad7190_CS_LOW();

uint8_t AD7190_ReadData(void);
void AD7190_Transmit(uint8_t data);

uint32_t AD7190_Read_Mode_Reg();
void AD7190_Write_ModeReg(uint32_t ModeReg_Val);

uint32_t AD7190_Read_Data_Reg();

uint32_t AD7190_Read_ConfigtureReg();
void AD7190_Write_ConfigureReg(uint32_t ConfigureReg_Val);


#endif


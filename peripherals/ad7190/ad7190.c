/*
 * 发送数据函数
 *
 * 复位函数
 *
 * 初始化函数：
 *      复位ad7190
 *      设置模式
 * 
 *
 *
 *
 *
 *
 *
 */



#include "ad7190.h"

#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>

#include "delay.h"

#define AD7190_DIN_HIGH HAL_GPIO_WritePin(AD7190_DIN_GPIO_Port, AD7190_DIN_Pin, GPIO_PIN_SET)
#define AD7190_DIN_LOW HAL_GPIO_WritePin(AD7190_DIN_GPIO_Port, AD7190_DIN_Pin, GPIO_PIN_RESET)

#define AD7190_SCLK_HIGH HAL_GPIO_WritePin(AD7190_SCLK_GPIO_Port, AD7190_SCLK_Pin, GPIO_PIN_SET)
#define AD7190_SCLK_LOW HAL_GPIO_WritePin(AD7190_SCLK_GPIO_Port, AD7190_SCLK_Pin, GPIO_PIN_RESET)


uint8_t ad7190_Dout_Read()
{
    return HAL_GPIO_ReadPin(AD7190_DOUT_GPIO_Port, AD7190_DOUT_Pin);
}

void ad7190_CS_HIGH()
{
    HAL_GPIO_WritePin(AD7190_CS_GPIO_Port, AD7190_CS_Pin, GPIO_PIN_SET);
    delay_us(1);
}


void ad7190_CS_LOW()
{
    HAL_GPIO_WritePin(AD7190_CS_GPIO_Port, AD7190_CS_Pin, GPIO_PIN_RESET);
    delay_us(1);
}


void AD7190_Transmit(uint8_t data)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {

        AD7190_SCLK_LOW;
        delay_us(1);

        if (data & 0x80) {
            AD7190_DIN_HIGH;
        }else {
            AD7190_DIN_LOW;
        }
        data <<= 1;

        AD7190_SCLK_HIGH;
        delay_us(1);

    }
}

uint8_t AD7190_ReadData(void)
{
    uint8_t ReadData = 0;
    uint8_t i = 0;
    for (i = 0; i < 8; i++) {

        AD7190_SCLK_LOW;
        delay_us(1);

        ReadData <<= 1;

        if (ad7190_Dout_Read()) {

            ReadData |= 0x01;
        }

        AD7190_SCLK_HIGH;
        delay_us(1);

    }
    return ReadData;
}


/*
 *
 *  读模式寄存器
 *
 *
 */
uint32_t AD7190_Read_Mode_Reg()
{
    ad7190_CS_LOW();

    uint32_t AD7190_Mode_Reg_Val = 0;
    AD7190_Transmit(0x48); // 先写通信寄存器，0 1 0 0 1 0 0 0 
    AD7190_Mode_Reg_Val |= AD7190_ReadData() << 16;
    AD7190_Mode_Reg_Val |= AD7190_ReadData() << 8;
    AD7190_Mode_Reg_Val |= AD7190_ReadData();

    ad7190_CS_HIGH();

    return AD7190_Mode_Reg_Val;

}


/* 
 * write mode register
 */

void AD7190_Write_ModeReg(uint32_t ModeReg_Val)
{
    ad7190_CS_LOW();

    //发送固定的通信寄存器命令
    AD7190_Transmit(0x08); // 0 | 0 | 001 | 0 | 00 -> 0000 1000 -> 0x08
    delay_us(1);

    // 发送数据 高8位
    uint8_t high_byte = (uint8_t)((ModeReg_Val >> 16) & 0xFF);
    AD7190_Transmit(high_byte);
    delay_us(1);

    // transmit middle byte
    uint8_t middle_byte = (uint8_t)((ModeReg_Val >> 8) & 0xFF);
    AD7190_Transmit(middle_byte);
    delay_us(1);

    // transmit low byte
    uint8_t low_byte = (uint8_t)(ModeReg_Val & 0xFF);
    AD7190_Transmit(low_byte);
    delay_us(1);

    ad7190_CS_HIGH();

    delay_us(5);

}


uint32_t AD7190_Read_Data_Reg()
{
    ad7190_CS_LOW();

    uint32_t AD7190_Data_Reg_Val = 0;
    AD7190_Transmit(0x58); // 先写通信寄存器，0 | 1 | 011 | 0 | 00 -> 0101 1000 -> 0x58 
    AD7190_Data_Reg_Val |= (uint32_t)AD7190_ReadData() << 16;
    AD7190_Data_Reg_Val |= (uint32_t)AD7190_ReadData() << 8;
    AD7190_Data_Reg_Val |= (uint32_t)AD7190_ReadData();

    ad7190_CS_HIGH();
    return AD7190_Data_Reg_Val;

}


uint32_t AD7190_Read_ConfigtureReg()
{
    ad7190_CS_LOW();

    uint32_t AD7190_Data_ConfigureVal = 0;
    AD7190_Transmit(0x50); // 先写通信寄存器，0 1 010 0 00
    AD7190_Data_ConfigureVal |= (uint32_t)AD7190_ReadData() << 16;
    AD7190_Data_ConfigureVal |= (uint32_t)AD7190_ReadData() << 8;
    AD7190_Data_ConfigureVal |= (uint32_t)AD7190_ReadData();

    ad7190_CS_HIGH();

    return AD7190_Data_ConfigureVal;

}
void AD7190_Write_ConfigureReg(uint32_t ConfigureReg_Val)
{
    ad7190_CS_LOW();

    //发送固定的通信寄存器命令
    AD7190_Transmit(0x10); // 0 0 010 000
    delay_us(1);

    // 发送数据 高8位
    uint8_t high_byte = (uint8_t)((ConfigureReg_Val >> 16) & 0xFF);
    AD7190_Transmit(high_byte);
    delay_us(1);

    // transmit middle byte
    uint8_t middle_byte = (uint8_t)((ConfigureReg_Val >> 8) & 0xFF);
    AD7190_Transmit(middle_byte);
    delay_us(1);

    // transmit low byte
    uint8_t low_byte = (uint8_t)(ConfigureReg_Val & 0xFF);
    AD7190_Transmit(low_byte);
    delay_us(1);

    ad7190_CS_HIGH();

    delay_us(5);

}


void AD7190_Restart()
{
    ad7190_CS_LOW();

    for (int i = 0; i<5; i++) {

        AD7190_Transmit(0xFF);

    }

    ad7190_CS_HIGH();

    delay_ms(1);

}


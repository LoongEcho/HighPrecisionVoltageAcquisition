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

void AD7190_Write_ModeReg(void)
{
    ad7190_CS_LOW();
    delay_us(1); // CS拉低后稳定

    // 1. 发送写模式寄存器命令（0x08）
    AD7190_Transmit(0x08);
    delay_us(1); // 字节间延时，确保数据稳定

    // 2. 发送高8位（0x00，保留位全0）
    AD7190_Transmit(0x00);
    delay_us(1);

    // 3. 发送中8位（0x00，FS高2位全0）
    AD7190_Transmit(0x00);
    delay_us(1);

    // 4. 发送低8位（0x61，单次转换模式）
    AD7190_Transmit(0x61);
    delay_us(1);

    // 关键：强制SCLK回到空闲高电平（CPOL=1），再拉高CS
    AD7190_SCLK_HIGH;
    delay_us(1);
    ad7190_CS_HIGH();
    delay_us(5); // 写操作后，给器件留时间处理配置
}
uint32_t AD7190_Read_ModeReg(void)
{
    uint32_t mode_reg = 0;
    uint8_t read_byte = 0;

    // 1. 拉低CS，选中AD7190（全程保持低电平，直到读取完成）
    ad7190_CS_LOW();
    delay_us(1); // 短暂延时，确保CS稳定拉低

    // 2. 发送读模式寄存器的通信命令（0x88 = 0 1 0 0 1 0 0 0）
    // CR7=0(WEN)、CR6=1(读)、CR5~CR3=001(模式寄存器地址)、CR2~CR0=000(保留位)
    AD7190_Transmit(0x88);
    delay_us(1); // 短暂延时，确保命令发送完成

    // 3. 读取3个字节（24位），高位先行，全程CS保持低电平
    // 第1个字节：高8位（BIT23~BIT16）
    read_byte = AD7190_ReadData();
    mode_reg |= (uint32_t)read_byte << 16; // 左移16位，存入高8位
    delay_us(1);

    // 第2个字节：中8位（BIT15~BIT8）
    read_byte = AD7190_ReadData();
    mode_reg |= (uint32_t)read_byte << 8; // 左移8位，存入中8位
    delay_us(1);

    // 第3个字节：低8位（BIT7~BIT0）
    read_byte = AD7190_ReadData();
    mode_reg |= (uint32_t)read_byte; // 直接存入低8位
    delay_us(1);

    // 4. 拉高CS，结束本次通信
    ad7190_CS_HIGH();

    // 5. 返回24位模式寄存器值
    return mode_reg;
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


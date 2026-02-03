/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "font.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "oled.h"

#include "ad7190.h"
#include <stdint.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  delay_init(72);
  delay_ms(20);
  OLED_Init();

  uint8_t show_buffer[10];
  uint8_t show_buffer2[10];
  uint8_t show_buffer3[10];
  uint8_t Show_Mode_Reg_buffer[10];

  uint8_t show_no_stuck_buffer[4];
  uint8_t AD7190_Status_Rig_Data;
  uint32_t AD7190_Mode_Reg_Val = 0;
  uint8_t show_no_stuck = 0;

  // 关键修改1：复位仅执行1次（上电后初始化，无需每次循环都执行）
  AD7190_Restart();
  delay_ms(1); // 复位后等待600μs（手册要求≥500μs），确保寄存器初始化完成

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
{
    OLED_NewFrame();

    // 第一次读取状态寄存器（复位后默认值0x80，正常）
    ad7190_CS_LOW();
    AD7190_Transmit(0x40);
    AD7190_Status_Rig_Data = AD7190_ReadData();
    ad7190_CS_HIGH();

    snprintf((char*)show_buffer, sizeof(show_buffer), "%02X", AD7190_Status_Rig_Data);
    OLED_PrintString(0, 0, "ADC Status:", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(112, 0, (char*)show_buffer, &font16x16, OLED_COLOR_NORMAL);
/*
    // 写入模式寄存器（单次转换模式）
    AD7190_Write_ModeReg();

    // 补充关键延时：确保寄存器生效+转换完成
    delay_us(5);
    delay_ms(500);

    // 读取模式寄存器
    uint32_t mode_reg = AD7190_Read_ModeReg();
    snprintf((char*)show_buffer3, sizeof(show_buffer3), "%06X", mode_reg);

    OLED_PrintString(0, 32, "Mode Reg:", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(112, 32, (char*)show_buffer3, &font16x16, OLED_COLOR_NORMAL);
*/
    // 第二次读取状态寄存器（预期转换完成，RDY位=0，显示00）
    ad7190_CS_LOW();
    AD7190_Transmit(0x40);         
    AD7190_Status_Rig_Data = AD7190_ReadData();
    ad7190_CS_HIGH();

    snprintf((char*)show_buffer2, sizeof(show_buffer2), "%02X", AD7190_Status_Rig_Data);
    OLED_PrintString(0, 16, "ADC Status2:", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(112, 16, (char*)show_buffer2, &font16x16, OLED_COLOR_NORMAL);


    // 读取模式寄存器
    ad7190_CS_LOW();
    AD7190_Transmit(0x48); // 先写通信寄存器，0 1 0 0 1 0 0 0 
    AD7190_Mode_Reg_Val |= AD7190_ReadData() << 16;
    AD7190_Mode_Reg_Val |= AD7190_ReadData() << 8;
    AD7190_Mode_Reg_Val |= AD7190_ReadData();

    snprintf((char*)Show_Mode_Reg_buffer, sizeof(Show_Mode_Reg_buffer), "%06X", AD7190_Mode_Reg_Val);
    OLED_PrintString(0, 32, "ModeReg:", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(80, 32, (char*)Show_Mode_Reg_buffer, &font16x16, OLED_COLOR_NORMAL);

    // 屏幕防卡死刷新
    show_no_stuck += 1;
    if (show_no_stuck > 20) {
        show_no_stuck = 0;
    }
    snprintf((char*)show_no_stuck_buffer, sizeof(show_no_stuck_buffer), "%02X", show_no_stuck);
    OLED_PrintString(96, 48, (char*)show_no_stuck_buffer, &font16x16, OLED_COLOR_NORMAL);

    delay_ms(500);
    OLED_ShowFrame();
}
  /* USER CODE END 3 */
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

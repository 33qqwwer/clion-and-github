/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
  // 定义跨文件共用的宏
#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t  // 可以顺便定义 u32，后续可能用到
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delay_us(uint32_t us);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_CS_Pin GPIO_PIN_10
#define SD_CS_GPIO_Port GPIOF
#define LCD_RST_Pin GPIO_PIN_0
#define LCD_RST_GPIO_Port GPIOC
#define CTP_INT_Pin GPIO_PIN_2
#define CTP_INT_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_3
#define LCD_RS_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_3
#define LCD_CS_GPIO_Port GPIOA
#define green_LED_Pin GPIO_PIN_0
#define green_LED_GPIO_Port GPIOB
#define CTP_RST_Pin GPIO_PIN_1
#define CTP_RST_GPIO_Port GPIOB
#define red_LED_Pin GPIO_PIN_14
#define red_LED_GPIO_Port GPIOB
#define yellow_LED_Pin GPIO_PIN_1
#define yellow_LED_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

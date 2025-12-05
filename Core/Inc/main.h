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
#define LCD_RST_Pin GPIO_PIN_0
#define LCD_RST_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_3
#define LCD_RS_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_3
#define LCD_CS_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define green_LED_Pin GPIO_PIN_0
#define green_LED_GPIO_Port GPIOB
#define red_LED_Pin GPIO_PIN_14
#define red_LED_GPIO_Port GPIOB
#define SPI1_MOSI_Pin GPIO_PIN_5
#define SPI1_MOSI_GPIO_Port GPIOB
#define yellow_LED_Pin GPIO_PIN_1
#define yellow_LED_GPIO_Port GPIOE
#define CTP_RST_Pin GPIO_PIN_8
#define CTP_RST_GPIO_Port GPIOI
#define CTP_INT_Pin GPIO_PIN_11
#define CTP_INT_GPIO_Port GPIOH

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

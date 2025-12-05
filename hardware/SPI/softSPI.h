#ifndef _SOFTSPI_H_
#define _SOFTSPI_H_

#include "main.h"
#include "gpio.h"
#include <stdint.h>  // 补充标准类型定义，替代u8

// SPI引脚定义（与main.h保持一致）
#define SPI1_MOSI_Pin     GPIO_PIN_5
#define SPI1_MOSI_GPIO_Port GPIOB
#define SPI1_SCK_Pin      GPIO_PIN_5
#define SPI1_SCK_GPIO_Port  GPIOA
#define SPI1_MISO_Pin     GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA

// 引脚操作宏（精简）
#define SPI_SCLK_SET    HAL_GPIO_WritePin(SPI1_SCK_GPIO_Port, SPI1_SCK_Pin, GPIO_PIN_SET)       
#define SPI_MOSI_SET    HAL_GPIO_WritePin(SPI1_MOSI_GPIO_Port, SPI1_MOSI_Pin, GPIO_PIN_SET)
#define SPI_SCLK_CLR    HAL_GPIO_WritePin(SPI1_SCK_GPIO_Port, SPI1_SCK_Pin, GPIO_PIN_RESET)
#define SPI_MOSI_CLR    HAL_GPIO_WritePin(SPI1_MOSI_GPIO_Port, SPI1_MOSI_Pin, GPIO_PIN_RESET)
#define SPI_MISO_READ   HAL_GPIO_ReadPin(SPI1_MISO_GPIO_Port, SPI1_MISO_Pin)

// SPI时序延迟配置（100MHz主频，可按需调整）
#define SOFTSPI_DELAY_US  1  // 1μs延迟 → SPI时钟≈500kHz（ST7796S稳定兼容）

// 函数声明
void SPI_WriteByte(uint8_t Byte);
uint8_t SPI_ReadByte(void);
uint8_t SPI_WriteReadByte(uint8_t tx_byte);
void SPI_GPIO_Init(void);
void SoftSPI_DelayUs(uint32_t us);  // 微秒延迟函数

#endif
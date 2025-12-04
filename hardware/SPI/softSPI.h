

#ifndef _softSPI_H_
#define _softSPI_H_


#include "main.h"
#include "GPIO.H"

// SPI引脚定义(在main.h文件里也有同样的定义，最好不要重复，只是测试的时候好看用)
#define SPI3_MOSI_Pin GPIO_PIN_2
#define SPI3_MOSI_GPIO_Port GPIOB
#define SPI3_SCK_Pin GPIO_PIN_10
#define SPI3_SCK_GPIO_Port GPIOC
#define SPI3_MISO_Pin GPIO_PIN_11
#define SPI3_MISO_GPIO_Port GPIOC




#define	SPI_SCLK_SET    HAL_GPIO_WritePin(SPI3_SCK_GPIO_Port,SPI3_SCK_Pin,GPIO_PIN_SET)
#define	SPI_MOSI_SET	HAL_GPIO_WritePin(SPI3_MOSI_GPIO_Port,SPI3_MOSI_Pin,GPIO_PIN_SET)


#define	SPI_SCLK_CLR   HAL_GPIO_WritePin(SPI3_SCK_GPIO_Port,SPI3_SCK_Pin,GPIO_PIN_RESET)
#define	SPI_MOSI_CLR	HAL_GPIO_WritePin(SPI3_MOSI_GPIO_Port,SPI3_MOSI_Pin,GPIO_PIN_RESET)

#define SPI_MISO_READ  HAL_GPIO_ReadPin(SPI3_MISO_GPIO_Port,SPI3_MISO_Pin)
 
void SPI_WriteByte(u8 Byte);
u8 SPI_ReadByte(void);
void SPI_GPIO_Init(void);




#endif

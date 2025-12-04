
#include "main.h"
#include "softSPI.h"


/*****************************************************************************
 * @name       :void SPI_WriteByte(u8 Byte)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using STM32's hardware SPI
 * @parameters :SPIx: SPI type,x for 1,2,3
                Byte:Data to be written
 * @retvalue   :Data received by the bus
******************************************************************************/
void SPI_WriteByte(u8 Byte)
{
	u8 i=0;
	for(i=0;i<8;i++)
	{
		if(Byte&0x80)
		{
			SPI_MOSI_SET;
		}
		else
		{
			SPI_MOSI_CLR;
		}
		SPI_SCLK_CLR;
		SPI_SCLK_SET;
		Byte<<=1;
	}
} 


/*****************************************************************************
 * @name       :u8 SPI_ReadByte(void)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using STM32's hardware SPI
 * @parameters :SPIx: SPI type,x for 1,2,3
                Byte:Data to be written
 * @retvalue   :Data received by the bus
******************************************************************************/
u8 SPI_ReadByte(void)
{
	u8 value=0,i=0,byte=0xFF;
	for(i=0;i<8;i++)
	{
		value<<=1;
		if(byte&0x80)
		{
			SPI_MOSI_SET;
		}
		else
		{
			SPI_MOSI_CLR;
		}
		byte<<=1;
		SPI_SCLK_CLR;
		HAL_Delay(100);
		if(SPI_MISO_READ)
		{
			value += 1;
		}
		SPI_SCLK_SET;	
		HAL_Delay(100);
	}
	return value;
} 

void SPI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* 启用SPI相关GPIO端口时钟 */
	__HAL_RCC_GPIOB_CLK_ENABLE(); // MOSI引脚
	__HAL_RCC_GPIOC_CLK_ENABLE(); // SCK引脚
	__HAL_RCC_GPIOA_CLK_ENABLE(); // MISO引脚

	/* 设置SPI引脚初始输出电平 */
	HAL_GPIO_WritePin(SPI3_SCK_GPIO_Port, SPI3_SCK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPI3_MOSI_GPIO_Port, SPI3_MOSI_Pin, GPIO_PIN_RESET);

	/* 配置SPI SCK和MOSI引脚为推挽输出模式 */
	GPIO_InitStruct.Pin = SPI3_SCK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPI3_SCK_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SPI3_MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPI3_MOSI_GPIO_Port, &GPIO_InitStruct);

	/* 配置SPI MISO引脚为输入模式 */
	GPIO_InitStruct.Pin = SPI3_MISO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(SPI3_MISO_GPIO_Port, &GPIO_InitStruct);
}


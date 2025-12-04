
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
	;
}

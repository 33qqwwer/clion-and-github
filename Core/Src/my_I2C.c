//
// Created by 艾伦 on 2025/11/3.
//
#include "my_I2C.h"


// #define my_I2C_SCK_h HAL_GPIO_WritePin(I2C_SCK_GPIO_Port,I2C_SCK_Pin,GPIO_PIN_SET)
// #define my_I2C_SCK_l HAL_GPIO_WritePin(I2C_SCK_GPIO_Port,I2C_SCK_Pin,GPIO_PIN_RESET)
// #define my_I2C_W_SDA HAL_GPIO_WritePin(I2C_SDA_GPIO_Port,I2C_SDA_Pin,GPIO_PIN_SET)
// #define my_I2C_R_SDA HAL_GPIO_ReadPin(i2c_SDA_GPIO_Port,I2C_SDA_Pin)

static void my_I2C_SCL(uint8_t Bitvalue)
{
	HAL_GPIO_WritePin(I2C_SCK_GPIO_Port,I2C_SCK_Pin,Bitvalue);
	HAL_Delay(5);
}

static void my_I2C_W_SDA(uint8_t Bitvalue)
{
	HAL_GPIO_WritePin(I2C_SDA_GPIO_Port,I2C_SDA_Pin,Bitvalue);
	HAL_Delay(5);
}

static uint8_t my_I2C_R_SDA(void )
{
	uint8_t Bitvalue=HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port,I2C_SDA_Pin);
	HAL_Delay(5);
	return Bitvalue;
}

void my_I2C_Start(void)
{
	my_I2C_SCL(1);
	my_I2C_W_SDA(1);
	my_I2C_W_SDA(0);
	my_I2C_SCL(0);
}
void my_I2C_Stop(void)
{
	my_I2C_W_SDA(0);
	my_I2C_SCL(0);
	my_I2C_SCL(1);
	my_I2C_W_SDA(1);
}

void my_I2C_sendByte(uint8_t Byte )
{
	for(uint8_t i=0;i<8;i++)
	{
		my_I2C_W_SDA(Byte&(0x80>>i));
		my_I2C_SCL(1);
		my_I2C_SCL(0);
	}

}
uint8_t my_I2C_reciveByte(void)
{
	uint8_t ReceiveByte=0x00;
	my_I2C_W_SDA(1);
	for(uint8_t i=0;i<8;i++)
	{
		my_I2C_SCL(1);
		if(my_I2C_R_SDA())
		{
			ReceiveByte|=(0x80>>i);
		}
		my_I2C_SCL(0);
	}
	return ReceiveByte;
}

void my_SendAck(uint8_t ACKBit)
{
	my_I2C_W_SDA(ACKBit);
	my_I2C_SCL(1);
	my_I2C_SCL(0);
}

uint8_t my_ReceiveAck(void)
{
	uint8_t ReceiveAck;
	my_I2C_W_SDA(1);
	my_I2C_SCL(1);
	ReceiveAck=my_I2C_R_SDA();
	my_I2C_SCL(0);
	return ReceiveAck;
}

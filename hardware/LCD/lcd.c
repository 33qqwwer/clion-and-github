
#include "lcd.h"
#include "usart.h"
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


uint8_t g_status[]="NO_OK\r\n";

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{
		LCD_CS_CLR;
		 LCD_RS_CLR;
	uint8_t status=HAL_SPI_Transmit(&hspi3, &data, 1, 10);
	if (status!=HAL_OK)
	{
		HAL_UART_Transmit(&huart3, g_status, sizeof(g_status), 10);
	}
		LCD_CS_SET;     // 5. 取消选中屏幕
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
	 LCD_CS_CLR;
	 LCD_RS_SET;
	uint8_t status=HAL_SPI_Transmit(&hspi3, &data, 1, 10);
	if (status!=HAL_OK)
	{
		HAL_UART_Transmit(&huart3,g_status, sizeof(g_status), 10);
	}
	LCD_CS_SET;
}

u8 LCD_RD_DATA(void)
{
	 u8 data;
	 LCD_CS_CLR;
	 LCD_RS_SET;
	uint8_t status= HAL_SPI_Receive(&hspi3, &data, 1, 10);
	if (status!=HAL_OK)
	{
		HAL_UART_Transmit(&huart3, g_status, sizeof(g_status), 10);
	}
	 LCD_CS_SET;
	 return data;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

u8 LCD_ReadReg(u8 LCD_Reg)
{
	LCD_WR_REG(LCD_Reg);
  return LCD_RD_DATA();
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 


void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{
	 LCD_CS_CLR;
	 LCD_RS_SET;
	uint8_t tx_buf[2];
	tx_buf[0] = (Data >> 8) & 0xFF;  // 高 8 位（比如 Data=0x1234 → tx_buf[0]=0x12）
	tx_buf[1] = Data & 0xFF;         // 低 8 位（比如 Data=0x1234 → tx_buf[1]=0x34）
	uint8_t status=HAL_SPI_Transmit(&hspi3, tx_buf, 2, 10);
	if (status!=HAL_OK)
	{
		HAL_UART_Transmit(&huart3, g_status, sizeof(g_status), 10);
	}
	 LCD_CS_SET;
}

u16 Lcd_ReadData_16Bit(void)
{
	uint8_t temp;
	uint16_t data;
	LCD_CS_CLR;
	LCD_RS_CLR;
	uint8_t status=HAL_SPI_Transmit(&hspi3, &lcddev.rramcmd, 1 ,10);  //ST7796U 的命令都是 8 位,
	if (status!=HAL_OK)
	{
		HAL_UART_Transmit(&huart3, g_status, sizeof(g_status), 10);
	}

	LCD_RS_SET;
	 status=HAL_SPI_Receive(&hspi3, &temp, 1 ,10);   //芯片手册规定读空一次
	if (status!=HAL_OK)
	{
		HAL_UART_Transmit(&huart3, g_status, sizeof(g_status), 10);
	}


	uint8_t tx_buf[2];
	status=HAL_SPI_Receive(&hspi3, tx_buf, 2 ,10);
	if (status!=HAL_OK)
	{
		HAL_UART_Transmit(&huart3, g_status, sizeof(g_status), 10);
	}
	data = tx_buf[0];
	data = (data << 8) | tx_buf[1];

	LCD_CS_SET;

	return data;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 color;
	LCD_SetCursor(x,y);//设置光标位置 
	color = Lcd_ReadData_16Bit();
	return color;
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
	uint8_t color_h=Color>>8 & 0xff;
	uint8_t color_l=Color & 0xff;
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
	    for(m=0;m<lcddev.width;m++)
	    {
		    HAL_SPI_Transmit(&hspi3,&color_h,1,10);
	    	HAL_SPI_Transmit(&hspi3,&color_l,1,10);
	    }
	}
	 LCD_CS_SET;
} 



/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	HAL_Delay(100);
	LCD_RST_SET;
	HAL_Delay(50);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{


	LCD_RESET(); //LCD 复位
	//*************3.5 ST7796S IPS初始化**********//
	LCD_WR_REG(0x11);

	HAL_Delay(120);                //Delay 120ms

	LCD_WR_REG(0x36);     // Memory Data Access Control MY,MX~~
	LCD_WR_DATA(0x48);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);

	LCD_WR_REG(0xF0);     // Command Set Control
	LCD_WR_DATA(0xC3);

	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x96);

	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x02);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0xC6);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0xC0);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x13);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA(0xA7);

	LCD_WR_REG(0xC5);
	LCD_WR_DATA(0x21);

	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x40);
	LCD_WR_DATA(0x8A);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x23);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0xAC);
	LCD_WR_DATA(0x33);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0xD2);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x46);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x15);
	LCD_WR_DATA(0x15);
	LCD_WR_DATA(0x2D);
	LCD_WR_DATA(0x32);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x96);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x25);
	LCD_WR_DATA(0x2E);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x42);
	LCD_WR_DATA(0x35);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x2E);

	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x3C);
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x69);

	HAL_Delay(120);
	LCD_WR_REG(0x21);
	LCD_WR_REG(0x29);

	LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
	LCD_Clear(WHITE);//清全屏白色
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
	lcddev.rramcmd=0x2E;
			lcddev.dir = direction%4;
	switch(lcddev.dir){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3)|(1<<6));
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<5));
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<7));
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<6)|(1<<5));
		break;	
		default:break;
	}		
}	 

u16 LCD_Read_ID(void)
{
	u8 i, tx_byte, rx_byte;
	u8 val[3] = {0};

	// 进入读取ID的命令集
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0xC3);
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x96);

	LCD_CS_CLR;

	for(i = 1; i < 4; i++)
	{
		// 发送命令0xFB
		LCD_RS_CLR;
		tx_byte = 0xFB;
		HAL_SPI_Transmit(&hspi3, &tx_byte, 1, 10);

		// 发送参数
		LCD_RS_SET;
		tx_byte = 0x10 + i;
		HAL_SPI_Transmit(&hspi3, &tx_byte, 1, 10);

		// 发送读取命令0xD3
		LCD_RS_CLR;
		tx_byte = 0xD3;
		HAL_SPI_Transmit(&hspi3, &tx_byte, 1, 10);

		// 读取数据
		LCD_RS_SET;
		HAL_SPI_Receive(&hspi3, &rx_byte, 1, 10);
		val[i - 1] = rx_byte;

		// 恢复状态
		LCD_RS_CLR;
		tx_byte = 0xFB;
		HAL_SPI_Transmit(&hspi3, &tx_byte, 1, 10);

		LCD_RS_SET;
		tx_byte = 0x00;
		HAL_SPI_Transmit(&hspi3, &tx_byte, 1, 10);
	}

	LCD_CS_SET;

	// 退出特殊命令集
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x3C);
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x69);

	// ST7796的ID通常是0x7796，格式是0xXYZW，其中XY是制造商，ZW是型号
	// 根据读取的数据组合
	lcddev.id = (val[1] << 8) | val[2];

	return lcddev.id;
}
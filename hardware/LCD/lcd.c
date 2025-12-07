
#include "lcd.h"
#include "stdlib.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "semphr.h"

// 自定义的delay_ms函数，不依赖HAL库
// 使用简单的循环实现延迟，确保快速执行


void delay_ms(uint32_t ms)
{
	osDelay(ms);
}

// 带延迟的LCD控制引脚操作函数
void LCD_CS_set(uint8_t state)
{
	state ? LCD_CS_SET : LCD_CS_CLR;
}

void LCD_RS_set(uint8_t state)
{
	state ? LCD_RS_SET : LCD_RS_CLR;
}

void LCD_RST_set(uint8_t state)
{
	state ? LCD_RST_SET : LCD_RST_CLR;
}

    
//定义LCD的参数
//默认为横屏
_lcd_dev lcddev;

//定义背景色和前景色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{ 
   LCD_CS_set(0);     
	 LCD_RS_set(0);	 
   SPI_WriteByte(data);
   LCD_CS_set(1);
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
   LCD_CS_set(0);
	 LCD_RS_set(1);
   SPI_WriteByte(data);
   LCD_CS_set(1);
}

u8 LCD_RD_DATA(void)
{ 
	 u8 data;
	 LCD_CS_set(0);
	 LCD_RS_set(1);
	 data = SPI_ReadByte();
	 LCD_CS_set(1);
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
	HAL_SPI_Transmit_DMA(&hspi1,&Data,2);
}

u16 Lcd_ReadData_16Bit(void)
{
	u16 r,g;
	LCD_CS_set(0);
	LCD_RS_set(0);	  
	SPI_WriteByte(lcddev.rramcmd);	
	LCD_RS_set(1);
	SPI_ReadByte();
	r = SPI_ReadByte();
	g = SPI_ReadByte();
	LCD_CS_set(1);
	r<<=8;
	r|=g;
	return r;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
// 绘制指定颜色的单个像素（适配LVGL动态颜色）
void LCD_DrawPoint(u16 x,u16 y,uint16_t color )
{
	if (x>=LCD_W ||y>=LCD_H)
	{
		return;
	}
	LCD_SetCursor(x,y);
	Lcd_WriteData_16Bit(color);
}

// 方案1：静态数组（全局/静态，堆上分配）
#define MAX_LINE_PIXEL 320 // 根据你的LCD宽度定义
static uint8_t color_buff8[MAX_LINE_PIXEL*2]; // 静态数组，仅初始化一次

//定义一个二值信号量
SemaphoreHandle_t SPI_DMA_handle;
// 批量绘制一行像素（提升刷新效率，关键！）
void LCD_DrawLine_Color(u16 x_start, u16 x_end, u16 y, u16 *color_buf, u16 len)
{
	// 1. 完整边界校验
	if(y >= LCD_H || x_start >= LCD_W || x_end >= LCD_W ||
	   x_start > x_end || len == 0 || color_buf == NULL) return;

	// 2. 计算实际可发送的16位像素数
	u16 real_pixel = x_end - x_start + 1;
	if(len < real_pixel) real_pixel = len;

	//由于是小端存储，必须调整数组内字节发送的顺序
	for (uint16_t i=0;i<real_pixel;i++)
	{
		color_buff8[i*2]=color_buf[i]>>8 & 0xff;
		color_buff8[i*2+1]=color_buf[i];
	}

	//使用DMA前先获取信号量
	xSemaphoreTake(SPI_DMA_handle,HAL_MAX_DELAY);

	// 3. 设置LCD窗口
	LCD_SetWindows(x_start, y, x_end, y);
	LCD_CS_set(0);
	LCD_RS_set(1);
	// 4. 发送拼接后的数据,拆开后的16位的数据高字节先发送，低字节后发送
	HAL_SPI_Transmit_DMA(&hspi1, color_buff8, real_pixel*2);

}



//spi的回调函数
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi==&hspi1)
	{
		LCD_CS_set(1);
			//发送完成后释放信号量
		xSemaphoreGiveFromISR(SPI_DMA_handle,NULL); //这里不需要,PdTRUE表示中断回调函数后执行一次任务调度,放信号量后若唤醒了更高优先级的任务
	}
}


u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 color;
	LCD_SetCursor(x,y);//
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
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_set(0);
	LCD_RS_set(1);
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
		SPI_WriteByte(Color>>8);
		SPI_WriteByte(Color);
	}
	}
	 LCD_CS_set(1);
} 

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 启用LCD相关GPIO端口时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE(); // LCD_CS引脚
    __HAL_RCC_GPIOC_CLK_ENABLE(); // LCD_RST和LCD_RS引脚

    /* 设置LCD引脚初始输出电平 */
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

    /* 配置LCD_CS引脚为推挽输出模式 */
    GPIO_InitStruct.Pin = LCD_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

    /* 配置LCD_RST引脚为推挽输出模式 */
    GPIO_InitStruct.Pin = LCD_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

    /* 配置LCD_RS引脚为推挽输出模式 */
    GPIO_InitStruct.Pin = LCD_RS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_RS_GPIO_Port, &GPIO_InitStruct);
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
	LCD_RST_set(0);  // LCD_RST_CLR + 1ms延迟
	osDelay(100);    // 保持总延迟100ms
	LCD_RST_set(1);  // LCD_RST_SET + 1ms延迟
	osDelay(50);    // 保持总延迟50ms
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
	//SPI_GPIO_Init(); //SPI GPIO初始化
	LCD_GPIOInit();//LCD GPIO初始化
	LCD_RESET(); //LCD 复位
	//*************3.5 ST7796S IPS初始化**********//
	LCD_WR_REG(0x11);

	delay_ms(120);                //Delay 120ms

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

	delay_ms(120);
	LCD_WR_REG(0x21);
	LCD_WR_REG(0x29);

	LCD_direction(USE_HORIZONTAL);//设置LCD显示方向

	//创建一个二值信号量
	SPI_DMA_handle=xSemaphoreCreateBinary();
	if (  SPI_DMA_handle!=NULL)
	{
		xSemaphoreGive(SPI_DMA_handle);
	}
	//LCD_Clear(BLUE);//清全屏白色
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

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
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
	u8 i,val[3] = {0};
	LCD_CS_set(0);  // 先拉低CS，确保所有指令发送到LCD
	LCD_WR_REG(0xF0);     // Command Set Control
	LCD_WR_DATA(0xC3);   

	LCD_WR_REG(0xF0);     
	LCD_WR_DATA(0x96);  
	LCD_CS_set(0);
	for(i=1;i<4;i++)
	{
		LCD_RS_set(0);	  
		SPI_WriteByte(0xFB);
		LCD_RS_set(1);
		SPI_WriteByte(0x10+i);
		LCD_RS_set(0);	  
		SPI_WriteByte(0xD3);
		LCD_RS_set(1);
		val[i-1] = SPI_ReadByte();
		LCD_RS_set(0);
		SPI_WriteByte(0xFB);
		LCD_RS_set(1);
		SPI_WriteByte(0x00);	
	}
	LCD_CS_set(1);
	LCD_WR_REG(0xF0);     // Command Set Control
	LCD_WR_DATA(0x3C);   
	LCD_WR_REG(0xF0);     
	LCD_WR_DATA(0x69);  
	lcddev.id=val[1];
	lcddev.id<<=8;
	lcddev.id|=val[2];
	return lcddev.id;
}
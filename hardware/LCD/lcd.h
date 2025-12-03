
#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "stdint.h"
#include "spi.h"



//LCD重要参数集
typedef struct
{
    u16 width;            //LCD 宽度
    u16 height;           //LCD 高度
    u16 id;               //LCD ID
    u8  dir;              //横屏还是竖屏控制：0，竖屏；1，横屏。
    u16 wramcmd;          //开始写gram指令
    u16 rramcmd;          //开始读gram指令
    u16 setxcmd;          //设置x坐标指令
    u16 setycmd;          //设置y坐标指令     
} _lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////
#define USE_HORIZONTAL  	 0//定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

/////////////////////////////////////?????????///////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 320
#define LCD_H 480

//画笔颜色
#define POINT_COLOR BLACK //默认红色
#define BACK_COLOR WHITE  //默认背景色为白色

////////////////////////////////////////////////////////////////////

#define	LCD_CS_SET		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#define	LCD_RS_SET		HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin, GPIO_PIN_SET)
#define	LCD_RST_SET		HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin, GPIO_PIN_SET)
#define SD_CS_SET		HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin, GPIO_PIN_SET)

#define	LCD_CS_CLR		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)//片选端口
#define	LCD_RS_CLR		HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin, GPIO_PIN_RESET) //数据/命令
#define	LCD_RST_CLR		HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin, GPIO_PIN_RESET)//复位
#define SD_CS_CLR		HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin, GPIO_PIN_RESET)



/************************************************/

//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			  	0XFFE0
#define GBLUE			  	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			  	0XBC40 //棕色
#define BRRED 			  	0XFC07 //棕红色
#define GRAY  			  	0X8430 //灰色
//GUI颜色

#define DARKBLUE       	 0X01CF	//深蓝色
#define LIGHTBLUE       	 0X7D7C	//浅蓝色  
#define GRAYBLUE        	 0X5458 //灰蓝色
//以上三色为PANEL的颜色
 
#define LIGHTGREEN      	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			  	0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE       	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
//常用LCD函数声明
void LCD_GPIOInit(void);
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);
void LCD_DrawPoint_big(u16 x,u16 y);
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_WR_REG(u8 data);
void LCD_WR_DATA(u8 data);
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_ReadRAM_Prepare(void);
void LCD_WriteData_16Bit(u16 Data);
u16 LCD_ReadData_16Bit(void);
void LCD_SetParam(void);
void LCD_SetArea(u16 x1, u16 y1, u16 x2, u16 y2);
u16 LCD_Read_ID(void);
void LCD_direction(u8 direction);
void LCD_LED(u8 on_off);
void LCD_RESET(void);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);
u16 LCD_BGR2RGB(u16 c);
#endif  
	 
	 




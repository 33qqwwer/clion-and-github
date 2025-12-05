
#include "ft6336.h"
#include "touch.h"
#include "i2c.h"
#include "string.h"
#include "lcd.h"
#include "stdio.h"
extern u8 touch_flag;

/*****************************************************************************
 * @name       :u8 FT5426_WR_Reg(u16 reg,u8 *buf,u8 len)
 * @date       :2020-05-13
 * @function   :Write data to ft5426 once
 * @parameters :reg:Start register address for written
								buf:the buffer of data written
								len:Length of data written
 * @retvalue   :0-Write succeeded
								1-Write failed
******************************************************************************/
u8 FT6336_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 ret=0;
	ret=HAL_I2C_Mem_Write(&hi2c1,FT_ADDRESS,reg,I2C_MEMADD_SIZE_8BIT,buf,len,200);
	return ret;
}

/*****************************************************************************
 * @name       :void FT5426_RD_Reg(u16 reg,u8 *buf,u8 len)
 * @date       :2020-05-13
 * @function   :Read data to ft5426 once
 * @parameters :reg:Start register address for read
								buf:the buffer of data read
								len:Length of data read
 * @retvalue   :none
******************************************************************************/
void FT6336_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	HAL_I2C_Mem_Read(&hi2c1,FT_ADDRESS,reg,I2C_MEMADD_SIZE_8BIT,buf,len,200);
}

/*****************************************************************************
 * @name       :u8 FT5426_Init(void)
 * @date       :2020-05-13
 * @function   :Initialize the ft5426 touch screen
 * @parameters :none
 * @retvalue   :0-Initialization successful
								1-initialization failed
******************************************************************************/
u8 FT6336_Init(void)
{
	u8 temp[2];

	FT_RST(0);				//复位
	HAL_Delay(10);
	FT_RST(1);				//释放复位
	HAL_Delay(500);

	// 1. 初始化FT6336工作模式
	temp[0]=0;
	FT6336_WR_Reg(FT_DEVIDE_MODE,temp,1);	//正常操作模式
	FT6336_WR_Reg(FT_ID_G_MODE,temp,1);		//查询模式
	temp[0]=40;								//触摸灵敏度（越小越灵敏）
	FT6336_WR_Reg(FT_ID_G_THGROUP,temp,1);

	// 2. 校验核心ID（Vendor ID固定为0x11）
	FT6336_RD_Reg(FT_ID_G_FOCALTECH_ID,&temp[0],1);
	if(temp[0]!=0x11)
	{
		printf("FT6336 Vendor ID Error:0x%02X\r\n",temp[0]);
		return 1;
	}

	// 3. 校验芯片型号（FT6336的MID=0x26，HID=0x64）
	FT6336_RD_Reg(FT_ID_G_CIPHER_MID,&temp[0],1);
	if(temp[0]!=0x26)
	{
		printf("FT6336 MID Error:0x%02X\r\n",temp[0]);
		return 1;
	}
	FT6336_RD_Reg(FT_ID_G_CIPHER_HIGH,&temp[0],1);
	if(temp[0]!=0x64)
	{
		printf("FT6336 HID Error:0x%02X\r\n",temp[0]);
		return 1;
	}

	// 4. 读取芯片版本（仅打印，不校验）
	FT6336_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);
	printf("FT6336 Version:0x%04X\r\n",((u16)temp[0]<<8)+temp[1]);

	// 5. 设置激活周期（12ms，范围12~14ms）
	temp[0]=12;
	FT6336_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);

	return 0;
}

const u16 FT6336_TPX_TBL[2]={FT_TP1_REG,FT_TP2_REG};

/*****************************************************************************
 * @name       :u8 FT5426_Scan(void)
 * @date       :2020-05-13
 * @function   :Scan touch screen (query mode)
 * @parameters :none
 * @retvalue   :Current touch screen status
								0-No touch
								1-With touch
******************************************************************************/


// uint8_t FT6336_Scan(void)
// {
//
//
// 	return 0;
// }

//成功返回1，失败返回0
u8 FT6336_Scan(void)
{
	u8 buf[4];
	u8 mode = 0;
	// 1. 每次都读触摸点数量
	FT6336_RD_Reg(FT_REG_NUM_FINGER,&mode,1);
	//printf("Touch Num: %d\r\n", mode); // 打印触摸点数（关键！）

	if(mode >=1 && mode <=2)
	{
		// 2. 读第一个触摸点
		FT6336_RD_Reg(FT_TP1_REG, buf, 4);
		// 3. 直接解析原始坐标（不转换）
		u16 x = ((u16)(buf[0]&0x0F)<<8) + buf[1];
		u16 y = ((u16)(buf[2]&0x0F)<<8) + buf[3];
		printf("Raw_X:%d, Raw_Y:%d\r\n", x, y); // 打印原始坐标

		// 4. 强制赋值tp_dev
		tp_dev.x[0] = x;
		tp_dev.y[0] = y;
		tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES | mode;
		return 1;
	}
	else
	{
		tp_dev.sta = 0;
		tp_dev.x[0] = 0xFFFF;
		tp_dev.y[0] = 0xFFFF;
		return 0;
	}
}



































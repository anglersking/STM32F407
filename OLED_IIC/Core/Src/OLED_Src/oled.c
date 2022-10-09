//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PD6（SCL）
//              D1   接PD7（SDA）
//              RES  接PD4
//              DC   接PD5
//              CS   接PD3               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "main.h"
#include "i2c.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   






void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);
}
//写数据
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}


//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_CMD(0xb0+y);
	OLED_WR_CMD(((x&0xf0)>>4)|0x10);
	OLED_WR_CMD((x&0x0f)|0x01); 
}   	  
//开启OLED显示    
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON
}
 
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF
}		

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  

void OLED_Clear()
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x02); 
		OLED_WR_CMD (0x10); 
		for(n=0;n<128;n++)
			OLED_WR_DATA(0x00);
	} 

}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_DATA(F8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_DATA(F8X16[c*16+i+8]);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_DATA(F6x8[c][i]);
				
			}
}
 

//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size); 
	}
} 
//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//显示汉字

void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_DATA(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_DATA(Hzk[2*no+1][t]);
				adder+=1;
      }					
}


//水平滚动
void OLED_ver_scroll()
{
  OLED_WR_CMD(0x2e);//关滚动
  OLED_WR_CMD(0x27);//29向右，2a向左
  OLED_WR_CMD(0x00);//A:空字节
  OLED_WR_CMD(0x02);//B:水平起始页
  OLED_WR_CMD(0x07);//C:水平滚动速度
  OLED_WR_CMD(0x03);//D:水平结束页
  OLED_WR_CMD(0x00);//E:每次垂直滚动位移
  OLED_WR_CMD(0xFF);//E:每次垂直滚动位移
  OLED_WR_CMD(0x2f);//开滚动
  

}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_DATA(BMP[j++]);	    	
	    }
	}
} 


//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
					  
	OLED_WR_CMD(0xAE);//--turn off oled panel
	OLED_WR_CMD(0x00);//---set low column address
	OLED_WR_CMD(0x10);//---set high column address
	OLED_WR_CMD(0x40);//--set start line address  Set Mapping RAM Display Start Line 
	OLED_WR_CMD(0x81);//--set contrast control register
	OLED_WR_CMD(0xCF);// Set SEG Output Current Brightness
	OLED_WR_CMD(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_CMD(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_CMD(0xA6);//--set normal display
	OLED_WR_CMD(0xA8);//--set multiplex ratio(1 to 64)
	OLED_WR_CMD(0x3f);//--1/64 duty
	OLED_WR_CMD(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_CMD(0x00);//-not offset
	OLED_WR_CMD(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WR_CMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_CMD(0xD9);//--set pre-charge period
	OLED_WR_CMD(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_CMD(0xDA);//--set com pins hardware configuration
	OLED_WR_CMD(0x12);
	OLED_WR_CMD(0xDB);//--set vcomh
	OLED_WR_CMD(0x40);//Set VCOM Deselect Level
	OLED_WR_CMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_CMD(0x02);//
	OLED_WR_CMD(0x8D);//--set Charge Pump enable/disable
	OLED_WR_CMD(0x14);//--set(0x10) disable
	OLED_WR_CMD(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_CMD(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OLED_Clear();
	OLED_WR_CMD(0xAF) ;/*display ON*/ 
	
}  






























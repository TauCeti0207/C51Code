#include <REGX52.H>
#include <INTRINS.H>
#include "DelayXms.h"
#include "LCD1602.H"
#include "MatrixKey.h"
#include "Timer0.h"
#include "Key.h"
#include "Key2.h"
#include "Buzzer.h"
#include "UART.h"
#include "MatrixLED.h"
#include "DS1302.h"
#include "OneWire.h"
#include "DS18B20.h"
#include "I2C.h"
#include "AT24C02.h"

unsigned char KeyNum, MODE, TimeSetSelect, TimeSetFlashFlag;

void TimeShow(void) //时间显示功能
{
	DS1302_ReadTime();					  //读取时间
	LCD_ShowNum(1, 1, DS1302_Time[0], 2); //显示年
	LCD_ShowNum(1, 4, DS1302_Time[1], 2); //显示月
	LCD_ShowNum(1, 7, DS1302_Time[2], 2); //显示日
	LCD_ShowNum(2, 1, DS1302_Time[3], 2); //显示时
	LCD_ShowNum(2, 4, DS1302_Time[4], 2); //显示分
	LCD_ShowNum(2, 7, DS1302_Time[5], 2); //显示秒
}

void TimeSet(void) //时间设置功能
{
	if (KeyNum == 2) //按键2按下
	{
		TimeSetSelect++;	// 设置选择位加1
		TimeSetSelect %= 6; // 越界清零
	}
	if (KeyNum == 3) //按键3按下
	{
		DS1302_Time[TimeSetSelect]++; //时间设置位数值加1
		if (DS1302_Time[0] > 99)
		{
			DS1302_Time[0] = 0;
		} //年越界判断
		if (DS1302_Time[1] > 12)
		{
			DS1302_Time[1] = 1;
		} //月越界判断
		if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
			DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) //日越界判断
		{
			if (DS1302_Time[2] > 31)
			{
				DS1302_Time[2] = 1;
			} //大月
		}
		else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
		{
			if (DS1302_Time[2] > 30)
			{
				DS1302_Time[2] = 1;
			} //小月
		}
		else if (DS1302_Time[1] == 2)
		{
			if (DS1302_Time[0] % 4 == 0) // 判断闰年
			{
				if (DS1302_Time[2] > 29)
				{
					DS1302_Time[2] = 1;
				} //闰年2月
			}
			else
			{
				if (DS1302_Time[2] > 28)
				{
					DS1302_Time[2] = 1;
				} //平年2月
			}
		}
		if (DS1302_Time[3] > 23)
		{
			DS1302_Time[3] = 0;
		} //时越界判断
		if (DS1302_Time[4] > 59)
		{
			DS1302_Time[4] = 0;
		} //分越界判断
		if (DS1302_Time[5] > 59)
		{
			DS1302_Time[5] = 0;
		} //秒越界判断
	}
	if (KeyNum == 4) //按键3按下
	{
		DS1302_Time[TimeSetSelect]--; //时间设置位数值减1
		if (DS1302_Time[0] < 0)
		{
			DS1302_Time[0] = 99;
		} //年越界判断
		if (DS1302_Time[1] < 1)
		{
			DS1302_Time[1] = 12;
		} //月越界判断
		if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
			DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) //日越界判断
		{
			if (DS1302_Time[2] < 1)
			{
				DS1302_Time[2] = 31;
			} //大月
			if (DS1302_Time[2] > 31)
			{
				DS1302_Time[2] = 1;
			}
		}
		else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
		{
			if (DS1302_Time[2] < 1)
			{
				DS1302_Time[2] = 30;
			} //小月
			if (DS1302_Time[2] > 30)
			{
				DS1302_Time[2] = 1;
			}
		}
		else if (DS1302_Time[1] == 2)
		{
			if (DS1302_Time[0] % 4 == 0)
			{
				if (DS1302_Time[2] < 1)
				{
					DS1302_Time[2] = 29;
				} //闰年2月
				if (DS1302_Time[2] > 29)
				{
					DS1302_Time[2] = 1;
				}
			}
			else
			{
				if (DS1302_Time[2] < 1)
				{
					DS1302_Time[2] = 28;
				} //平年2月
				if (DS1302_Time[2] > 28)
				{
					DS1302_Time[2] = 1;
				}
			}
		}
		if (DS1302_Time[3] < 0)
		{
			DS1302_Time[3] = 23;
		} //时越界判断
		if (DS1302_Time[4] < 0)
		{
			DS1302_Time[4] = 59;
		} //分越界判断
		if (DS1302_Time[5] < 0)
		{
			DS1302_Time[5] = 59;
		} //秒越界判断
	}

	// 设置完之后更新显示，根据TimeSetSelect和TimeSetFlashFlag判断可完成闪烁功能
	if (TimeSetSelect == 0 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 1, "  ");
	}
	else
	{
		LCD_ShowNum(1, 1, DS1302_Time[0], 2);
	}
	if (TimeSetSelect == 1 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 4, "  ");
	}
	else
	{
		LCD_ShowNum(1, 4, DS1302_Time[1], 2);
	}
	if (TimeSetSelect == 2 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 7, "  ");
	}
	else
	{
		LCD_ShowNum(1, 7, DS1302_Time[2], 2);
	}
	if (TimeSetSelect == 3 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 1, "  ");
	}
	else
	{
		LCD_ShowNum(2, 1, DS1302_Time[3], 2);
	}
	if (TimeSetSelect == 4 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 4, "  ");
	}
	else
	{
		LCD_ShowNum(2, 4, DS1302_Time[4], 2);
	}
	if (TimeSetSelect == 5 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 7, "  ");
	}
	else
	{
		LCD_ShowNum(2, 7, DS1302_Time[5], 2);
	}
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18; //设置定时初值
	TH0 = 0xFC; //设置定时初值
	T0Count++;
	if (T0Count >= 500) //每500ms进入一次
	{
		T0Count = 0;
		TimeSetFlashFlag = !TimeSetFlashFlag; //闪烁标志位取反
	}
}

// void Timer0_Routine() interrupt 1
// {
// 	static unsigned int T0Count;
// 	TL0 = 0x18; //设置定时初值
// 	TH0 = 0xFC; //设置定时初值
// 	T0Count++;
// 	if (T0Count >= 20)
// 	{
// 		T0Count = 0;
// 		Key_Loop(); //每20ms调用一次按键驱动函数
// 	}
// }

float T, TShow;
char TLow, THigh;
unsigned char KeyNum2;

void TemperatureMoudle()
{
	DS18B20_ConvertT();			 //上电先转换一次温度，防止第一次读数据错误
	Delay(1000);				 //等待转换完成
	THigh = AT24C02_ReadByte(0); //读取温度阈值数据
	TLow = AT24C02_ReadByte(1);
	if (THigh > 125 || TLow < -55 || THigh <= TLow)
	{
		THigh = 20; //如果阈值非法，则设为默认值
		TLow = 15;
	}
	LCD_Init();
	LCD_ShowString(1, 1, "T:");
	LCD_ShowString(2, 1, "TH:");
	LCD_ShowString(2, 9, "TL:");
	LCD_ShowSignedNum(2, 4, THigh, 3);
	LCD_ShowSignedNum(2, 12, TLow, 3);
	Timer0Init();

	while (1)
	{
		KeyNum2 = Key();

		/*温度读取及显示*/
		DS18B20_ConvertT();	 //转换温度
		T = DS18B20_ReadT(); //读取温度
		if (T < 0)			 //如果温度小于0
		{
			LCD_ShowChar(1, 3, '-'); //显示负号
			TShow = -T;				 //将温度变为正数
		}
		else //如果温度大于等于0
		{
			LCD_ShowChar(1, 3, '+'); //显示正号
			TShow = T;
		}
		LCD_ShowNum(1, 4, TShow, 3);							  //显示温度整数部分
		LCD_ShowChar(1, 7, '.');								  //显示小数点
		LCD_ShowNum(1, 8, (unsigned long)(TShow * 100) % 100, 2); //显示温度小数部分

		/*阈值判断及显示*/
		if (KeyNum2)
		{
			if (KeyNum2 == 1) // K1按键，THigh自增
			{
				THigh++;
				if (THigh > 125)
				{
					THigh = 125;
				}
			}
			if (KeyNum2 == 2) // K2按键，THigh自减
			{
				THigh--;
				if (THigh <= TLow)
				{
					THigh++;
				}
			}
			if (KeyNum2 == 3) // K3按键，TLow自增
			{
				TLow++;
				if (TLow >= THigh)
				{
					TLow--;
				}
			}
			if (KeyNum2 == 4) // K4按键，TLow自减
			{
				TLow--;
				if (TLow < -55)
				{
					TLow = -55;
				}
			}
			LCD_ShowSignedNum(2, 4, THigh, 3); // 显示阈值数据
			LCD_ShowSignedNum(2, 12, TLow, 3);
			AT24C02_WriteByte(0, THigh); // 写入到AT24C02中保存
			Delay(5);
			AT24C02_WriteByte(1, TLow);
			Delay(5);
		}

		// 高温或低温均会报警
		if (T > THigh) //越界判断
		{
			Buzzer_Time(500);
			LCD_ShowString(1, 13, "OV:H");
		}
		else if (T < TLow)
		{
			Buzzer_Time(500);
			LCD_ShowString(1, 13, "OV:L");
		}
		else
		{
			LCD_ShowString(1, 13, "    ");
		}
	}
}

void TimeMoudle()
{
	LCD_Init();
	DS1302_Init();
	Timer0Init();
	LCD_ShowString(1, 1, "  -  -  "); //静态字符初始化显示
	LCD_ShowString(2, 1, "  :  :  ");
	DS1302_SetTime(); //设置时间

	while (1)
	{
		if (DS1302_Time[4] == 59 && DS1302_Time[5] == 59)
		{
			Buzzer_Time(1000); // 整点报时1s
		}
		KeyNum = MatrixKey(); //读取键码
		if (KeyNum == 1)	  //按键1按下
		{
			if (MODE == 0)
			{
				MODE = 1;
				TimeSetSelect = 0;
			} //功能切换
			else if (MODE == 1)
			{
				MODE = 0;
				DS1302_SetTime();
			}
		}
		if (MODE == 0) // 根据不同的功能执行不同的函数
		{
			TimeShow();
		}
		if (MODE == 1)
		{
			TimeSet();
		}
		// 通过复位重新选择功能
	}
}

void StopWatch()
{
	int Result = 0;
	LCD_Init();
	while (1)
	{
		Result++;	 // Result自增
		Delay(1000); //延时1秒
		LCD_ShowString(1, 4, "            ");
		LCD_ShowString(2, 1, "               ");
		LCD_ShowNum(1, 1, Result, 3); //在LCD的1行1列显示Result，长度为3位
	}
}

void CountDown1()
{
	int Result = 60;
	LCD_Init();
	while (1)
	{
		Delay(1000); //延时1秒
		LCD_ShowString(1, 4, "            ");
		LCD_ShowString(2, 1, "               ");
		LCD_ShowNum(1, 1, Result, 3); //在LCD的1行1列显示Result，长度为3位
		Result--;
	}
}

void CountDown2()
{
	int Result = 180;
	LCD_Init();
	while (1)
	{
		Delay(1000); //延时1秒
		LCD_ShowString(1, 4, "            ");
		LCD_ShowString(2, 1, "               ");
		LCD_ShowNum(1, 1, Result, 3); //在LCD的1行1列显示Result，长度为3位
		Result--;
	}
}


void main()
{
	KeyNum = MatrixKey();
	switch (KeyNum)
	{
	case 5:
		TimeMoudle();
		break;
	case 6:
		TemperatureMoudle();
		break;
	case 7:
		StopWatch();
		break;
	case 8:
		CountDown1();
		break;
	case 9:
		CountDown2();
		break;
	default:
		break;
	}
}
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

void TimeShow(void) //ʱ����ʾ����
{
	DS1302_ReadTime();					  //��ȡʱ��
	LCD_ShowNum(1, 1, DS1302_Time[0], 2); //��ʾ��
	LCD_ShowNum(1, 4, DS1302_Time[1], 2); //��ʾ��
	LCD_ShowNum(1, 7, DS1302_Time[2], 2); //��ʾ��
	LCD_ShowNum(2, 1, DS1302_Time[3], 2); //��ʾʱ
	LCD_ShowNum(2, 4, DS1302_Time[4], 2); //��ʾ��
	LCD_ShowNum(2, 7, DS1302_Time[5], 2); //��ʾ��
}

void TimeSet(void) //ʱ�����ù���
{
	if (KeyNum == 2) //����2����
	{
		TimeSetSelect++;	// ����ѡ��λ��1
		TimeSetSelect %= 6; // Խ������
	}
	if (KeyNum == 3) //����3����
	{
		DS1302_Time[TimeSetSelect]++; //ʱ������λ��ֵ��1
		if (DS1302_Time[0] > 99)
		{
			DS1302_Time[0] = 0;
		} //��Խ���ж�
		if (DS1302_Time[1] > 12)
		{
			DS1302_Time[1] = 1;
		} //��Խ���ж�
		if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
			DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) //��Խ���ж�
		{
			if (DS1302_Time[2] > 31)
			{
				DS1302_Time[2] = 1;
			} //����
		}
		else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
		{
			if (DS1302_Time[2] > 30)
			{
				DS1302_Time[2] = 1;
			} //С��
		}
		else if (DS1302_Time[1] == 2)
		{
			if (DS1302_Time[0] % 4 == 0) // �ж�����
			{
				if (DS1302_Time[2] > 29)
				{
					DS1302_Time[2] = 1;
				} //����2��
			}
			else
			{
				if (DS1302_Time[2] > 28)
				{
					DS1302_Time[2] = 1;
				} //ƽ��2��
			}
		}
		if (DS1302_Time[3] > 23)
		{
			DS1302_Time[3] = 0;
		} //ʱԽ���ж�
		if (DS1302_Time[4] > 59)
		{
			DS1302_Time[4] = 0;
		} //��Խ���ж�
		if (DS1302_Time[5] > 59)
		{
			DS1302_Time[5] = 0;
		} //��Խ���ж�
	}
	if (KeyNum == 4) //����3����
	{
		DS1302_Time[TimeSetSelect]--; //ʱ������λ��ֵ��1
		if (DS1302_Time[0] < 0)
		{
			DS1302_Time[0] = 99;
		} //��Խ���ж�
		if (DS1302_Time[1] < 1)
		{
			DS1302_Time[1] = 12;
		} //��Խ���ж�
		if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
			DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) //��Խ���ж�
		{
			if (DS1302_Time[2] < 1)
			{
				DS1302_Time[2] = 31;
			} //����
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
			} //С��
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
				} //����2��
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
				} //ƽ��2��
				if (DS1302_Time[2] > 28)
				{
					DS1302_Time[2] = 1;
				}
			}
		}
		if (DS1302_Time[3] < 0)
		{
			DS1302_Time[3] = 23;
		} //ʱԽ���ж�
		if (DS1302_Time[4] < 0)
		{
			DS1302_Time[4] = 59;
		} //��Խ���ж�
		if (DS1302_Time[5] < 0)
		{
			DS1302_Time[5] = 59;
		} //��Խ���ж�
	}

	// ������֮�������ʾ������TimeSetSelect��TimeSetFlashFlag�жϿ������˸����
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
	TL0 = 0x18; //���ö�ʱ��ֵ
	TH0 = 0xFC; //���ö�ʱ��ֵ
	T0Count++;
	if (T0Count >= 500) //ÿ500ms����һ��
	{
		T0Count = 0;
		TimeSetFlashFlag = !TimeSetFlashFlag; //��˸��־λȡ��
	}
}

// void Timer0_Routine() interrupt 1
// {
// 	static unsigned int T0Count;
// 	TL0 = 0x18; //���ö�ʱ��ֵ
// 	TH0 = 0xFC; //���ö�ʱ��ֵ
// 	T0Count++;
// 	if (T0Count >= 20)
// 	{
// 		T0Count = 0;
// 		Key_Loop(); //ÿ20ms����һ�ΰ�����������
// 	}
// }

float T, TShow;
char TLow, THigh;
unsigned char KeyNum2;

void TemperatureMoudle()
{
	DS18B20_ConvertT();			 //�ϵ���ת��һ���¶ȣ���ֹ��һ�ζ����ݴ���
	Delay(1000);				 //�ȴ�ת�����
	THigh = AT24C02_ReadByte(0); //��ȡ�¶���ֵ����
	TLow = AT24C02_ReadByte(1);
	if (THigh > 125 || TLow < -55 || THigh <= TLow)
	{
		THigh = 20; //�����ֵ�Ƿ�������ΪĬ��ֵ
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

		/*�¶ȶ�ȡ����ʾ*/
		DS18B20_ConvertT();	 //ת���¶�
		T = DS18B20_ReadT(); //��ȡ�¶�
		if (T < 0)			 //����¶�С��0
		{
			LCD_ShowChar(1, 3, '-'); //��ʾ����
			TShow = -T;				 //���¶ȱ�Ϊ����
		}
		else //����¶ȴ��ڵ���0
		{
			LCD_ShowChar(1, 3, '+'); //��ʾ����
			TShow = T;
		}
		LCD_ShowNum(1, 4, TShow, 3);							  //��ʾ�¶���������
		LCD_ShowChar(1, 7, '.');								  //��ʾС����
		LCD_ShowNum(1, 8, (unsigned long)(TShow * 100) % 100, 2); //��ʾ�¶�С������

		/*��ֵ�жϼ���ʾ*/
		if (KeyNum2)
		{
			if (KeyNum2 == 1) // K1������THigh����
			{
				THigh++;
				if (THigh > 125)
				{
					THigh = 125;
				}
			}
			if (KeyNum2 == 2) // K2������THigh�Լ�
			{
				THigh--;
				if (THigh <= TLow)
				{
					THigh++;
				}
			}
			if (KeyNum2 == 3) // K3������TLow����
			{
				TLow++;
				if (TLow >= THigh)
				{
					TLow--;
				}
			}
			if (KeyNum2 == 4) // K4������TLow�Լ�
			{
				TLow--;
				if (TLow < -55)
				{
					TLow = -55;
				}
			}
			LCD_ShowSignedNum(2, 4, THigh, 3); // ��ʾ��ֵ����
			LCD_ShowSignedNum(2, 12, TLow, 3);
			AT24C02_WriteByte(0, THigh); // д�뵽AT24C02�б���
			Delay(5);
			AT24C02_WriteByte(1, TLow);
			Delay(5);
		}

		// ���»���¾��ᱨ��
		if (T > THigh) //Խ���ж�
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
	LCD_ShowString(1, 1, "  -  -  "); //��̬�ַ���ʼ����ʾ
	LCD_ShowString(2, 1, "  :  :  ");
	DS1302_SetTime(); //����ʱ��

	while (1)
	{
		if (DS1302_Time[4] == 59 && DS1302_Time[5] == 59)
		{
			Buzzer_Time(1000); // ���㱨ʱ1s
		}
		KeyNum = MatrixKey(); //��ȡ����
		if (KeyNum == 1)	  //����1����
		{
			if (MODE == 0)
			{
				MODE = 1;
				TimeSetSelect = 0;
			} //�����л�
			else if (MODE == 1)
			{
				MODE = 0;
				DS1302_SetTime();
			}
		}
		if (MODE == 0) // ���ݲ�ͬ�Ĺ���ִ�в�ͬ�ĺ���
		{
			TimeShow();
		}
		if (MODE == 1)
		{
			TimeSet();
		}
		// ͨ����λ����ѡ����
	}
}

void StopWatch()
{
	int Result = 0;
	LCD_Init();
	while (1)
	{
		Result++;	 // Result����
		Delay(1000); //��ʱ1��
		LCD_ShowString(1, 4, "            ");
		LCD_ShowString(2, 1, "               ");
		LCD_ShowNum(1, 1, Result, 3); //��LCD��1��1����ʾResult������Ϊ3λ
	}
}

void CountDown1()
{
	int Result = 60;
	LCD_Init();
	while (1)
	{
		Delay(1000); //��ʱ1��
		LCD_ShowString(1, 4, "            ");
		LCD_ShowString(2, 1, "               ");
		LCD_ShowNum(1, 1, Result, 3); //��LCD��1��1����ʾResult������Ϊ3λ
		Result--;
	}
}

void CountDown2()
{
	int Result = 180;
	LCD_Init();
	while (1)
	{
		Delay(1000); //��ʱ1��
		LCD_ShowString(1, 4, "            ");
		LCD_ShowString(2, 1, "               ");
		LCD_ShowNum(1, 1, Result, 3); //��LCD��1��1����ʾResult������Ϊ3λ
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
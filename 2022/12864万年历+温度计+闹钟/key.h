#ifndef _KEY
#define _KEY
/***********************键盘预定义******************************************/
#define up_array 0x01
#define down_array 0x02
#define clear_array 0x03
#define function_array 0x04
#define null 0

/********************************************************************************************
 * 函数名称：Key_Idle()
 * 功    能：键盘松开
 * 入口参数：无
 * 出口参数：无
 *********************************************************************************************/
void Key_Idle()
{
	while ((P0 & 0x0f) != 0x0f)
		;
}

/********************************************************************************************
 * 函数名称：Key_Scan()
 * 功    能：键盘扫描
 * 入口参数：无
 * 出口参数：键值
 *********************************************************************************************/
uchar Key_Scan()
{
	if ((P0 & 0x0f) != 0x0f) //判断按键
	{
		Delay(4); //消除抖动
		if ((P0 & 0x0f) != 0x0f)
		{

			switch (P0 & 0x0f) //将按键码转换成键值
			{
			case 0x0e:
				return up_array;
			case 0x0d:
				return down_array;
			case 0x0b:
				return clear_array;
			case 0x07:
				return function_array;
			default:
				return null;
			}
		}
	}
	return null;
}

/********************************************************************************************
 * 函数名称：Key_Set_Clock( uchar * clock_time )
 * 功    能：键盘设定时间
 * 入口参数：clock_time[6]指针
 * 出口参数：无
 *********************************************************************************************/
void Key_Set_Clock(uchar *clock_time)
{
	uchar *i;
	i = clock_time;
	clock_time++;		 //指向分
	Lcd_Set_Clock(2, i); //屏幕显示更该时间
	Key_Idle();
	while (1)
	{
		uchar minute;
		minute = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD 转 HEX
		switch (Key_Scan())											 //调整分
		{
		case up_array:
		{
			minute++;
			if (minute == 60)
				minute = 0;
			(*clock_time) = minute / 10 << 4;
			(*clock_time) = (*clock_time) | (minute % 10); // HEX 转 BCD
			Lcd_Set_Clock(2, i);						   //屏幕显示更该时间
			Key_Idle();
		}
		break;
		case down_array:
		{
			if (minute == 0)
				minute = 60;
			minute--;
			(*clock_time) = minute / 10 << 4;
			(*clock_time) = (*clock_time) | (minute % 10); // HEX 转 BCD
			Lcd_Set_Clock(2, i);						   //屏幕显示更该时间
			Key_Idle();
		}
		break;
			//******************************************************************************************************************************************************
		case function_array: //调整时
		{
			clock_time++;		 //指向时
			Lcd_Set_Clock(3, i); //屏幕显示更该时间
			Key_Idle();
			while (1)
			{
				uchar hour;
				hour = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD 转 HEX
				switch (Key_Scan())
				{
				case up_array:
				{
					hour++;
					if (hour == 24)
						hour = 0;
					(*clock_time) = hour / 10 << 4;
					(*clock_time) = (*clock_time) | (hour % 10); // HEX 转 BCD
					Lcd_Set_Clock(3, i);						 //屏幕显示更该时间
					Key_Idle();
				}
				break;

				case down_array:
				{
					if (hour == 0)
						hour = 24;
					hour--;
					if (hour == 0)
						hour = 23;
					(*clock_time) = hour / 10 << 4;
					(*clock_time) = (*clock_time) | (hour % 10); // HEX 转 BCD
					Lcd_Set_Clock(3, i);						 //屏幕显示更该时间
					Key_Idle();
				}
				break;
					//******************************************************************************************************************************************************
				case function_array: //调整日
				{
					clock_time++;		 //指向日
					Lcd_Set_Clock(4, i); //屏幕显示更该时间
					Key_Idle();
					while (1)
					{
						uchar day;
						day = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD 转 HEX
						switch (Key_Scan())
						{
						case up_array:
						{
							day++;
							if (day == 32)
								day = 1;
							(*clock_time) = day / 10 << 4;
							(*clock_time) = (*clock_time) | (day % 10); // HEX 转 BCD
							Lcd_Set_Clock(4, i);						//屏幕显示更该时间
							Key_Idle();
						}
						break;
						case down_array:
						{
							if (day == 0)
								day = 31;
							day--;
							if (day == 0)
								day = 31;
							(*clock_time) = day / 10 << 4;
							(*clock_time) = (*clock_time) | (day % 10); // HEX 转 BCD ;
							Lcd_Set_Clock(4, i);						//屏幕显示更该时间
							Key_Idle();
						}
						break;
							//******************************************************************************************************************************************************
						case function_array: //调整月
						{
							clock_time++;		 //指向月
							Lcd_Set_Clock(5, i); //屏幕显示更该时间
							Key_Idle();
							while (1)
							{
								uchar month;
								month = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD 转 HEX
								switch (Key_Scan())
								{
								case up_array:
								{
									month++;
									if (month == 13)
										month = 1;
									(*clock_time) = month / 10 << 4;
									(*clock_time) = (*clock_time) | (month % 10); // HEX 转 BCD
									Lcd_Set_Clock(5, i);						  //屏幕显示更该时间
									Key_Idle();
								}
								break;
								case down_array:
								{
									if (month == 0)
										month = 12;
									month--;
									if (month == 0)
										month = 12;
									(*clock_time) = month / 10 << 4;
									(*clock_time) = (*clock_time) | (month % 10); // HEX 转 BCD
									Lcd_Set_Clock(5, i);						  //屏幕显示更该时间
									Key_Idle();
								}
								break;
									//******************************************************************************************************************************************************
								case function_array: //调整年
								{
									clock_time++;		 //指向年
									Lcd_Set_Clock(6, i); //屏幕显示更该时间
									Key_Idle();
									while (1)
									{
										uchar year;
										year = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD 转 HEX
										switch (Key_Scan())
										{
										case up_array:
										{
											year++;
											if (year == 100)
												year = 0;
											(*clock_time) = year / 10 << 4;
											(*clock_time) = (*clock_time) | (year % 10); // HEX 转 BCD
											Lcd_Set_Clock(6, i);						 //屏幕显示更该时间
											Key_Idle();
										}
										break;
										case down_array:
										{
											if (year == 0)
												year = 100;
											year--;
											if (year == 0)
												year = 100;
											(*clock_time) = year / 10 << 4;
											(*clock_time) = (*clock_time) | (year % 10); // HEX 转 BCD
											Lcd_Set_Clock(6, i);						 //屏幕显示更该时间
											Key_Idle();
										}
										break;
											//******************************************************************************************************************************************************
										case function_array:
										{
											goto loop;
										}
										break;
										}
									}
								}
								}
							}
						}
						}
					}
				}
				}
			}
		}
		}
	}
loop:
	Key_Idle();
}
/***************************************************************************************
 * 名称: Key_Set_Alarm( uchar *  alarm_time )
 * 功能: 闹钟调整
 * 入口参数: 闹钟地址
 * 出口参数:
 ****************************************************************************************/
void Key_Set_Alarm(uchar *alarm_time)
{
	Lcd_Alarm(1, alarm_time);
	Key_Idle();
	while (1)
	{
		switch (Key_Scan())
		{
		case up_array:
		{
			(*alarm_time)++;
			if (*alarm_time == 60)
				*alarm_time = 0;
			Lcd_Alarm(1, alarm_time);
			Key_Idle();
		}
		break;
		case down_array:
		{
			if (*alarm_time == 0)
				*alarm_time = 60;
			(*alarm_time)--;
			Lcd_Alarm(1, alarm_time);
			Key_Idle();
		}
		break;
		case clear_array:
		{
			Lcd_Clear();
			goto loop;
		}
		break;
		case function_array:
		{
			alarm_time++;
			Lcd_Alarm(2, alarm_time - 1);
			Key_Idle();
			while (1)
			{
				switch (Key_Scan())
				{
				case up_array:
				{
					(*alarm_time)++;
					if (*alarm_time == 24)
						*alarm_time = 0;
					Lcd_Alarm(2, alarm_time - 1);
					Key_Idle();
				}
				break;
				case down_array:
				{
					if (*alarm_time == 0)
						*alarm_time = 24;
					(*alarm_time)--;
					Lcd_Alarm(2, alarm_time - 1);
					Key_Idle();
				}
				break;
				case clear_array:
				{
					Lcd_Clear();
					goto loop;
				}
				break;
				case function_array:
				{
					Lcd_Clear();
					goto loop;
				}
				break;
				}
			}
		}
		break;
		}
	}
loop:
	Key_Idle();
}
/****************************************************************************************
 * 名称: Key_Function( uchar * clock_time )
 * 功能: 系统功能调试
 * 入口参数: 时钟程序
 * 出口参数: 无
 *****************************************************************************************/
void Key_Function(uchar *clock_time, uchar *alarm_time)
{
	uchar temp = 2;
	Lcd_Clear();
	Lcd_Function(temp);
	Key_Idle();
	while (1)
	{
		switch (Key_Scan())
		{
		case up_array:
		{
			temp++;
			if (temp > 2)
				temp = 2;
			Lcd_Function(temp);
			Key_Idle();
		}
		break;
		case down_array:
		{
			temp--;
			if (temp == 0)
				temp = 1;
			Lcd_Function(temp);
			Key_Idle();
		}
		break;
		case clear_array:
		{
			Lcd_Clear();
			goto loop;
		}
		break;
		case function_array:
		{
			if (temp == 2)
			{
				Lcd_Clear();
				Key_Set_Clock(clock_time);
				Clock_Initial(clock_time);
				Lcd_Clear();
				goto loop;
			}
			if (temp == 1)
			{
				Lcd_Clear();
				Key_Set_Alarm(alarm_time);
				Lcd_Clear();
				goto loop;
			}
		}
		break;
		}
	}
loop:
	Key_Idle();
}
/********************************************************************************/
#endif
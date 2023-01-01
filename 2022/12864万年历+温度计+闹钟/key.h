#ifndef _KEY
#define _KEY
/***********************����Ԥ����******************************************/
#define up_array 0x01
#define down_array 0x02
#define clear_array 0x03
#define function_array 0x04
#define null 0

/********************************************************************************************
 * �������ƣ�Key_Idle()
 * ��    �ܣ������ɿ�
 * ��ڲ�������
 * ���ڲ�������
 *********************************************************************************************/
void Key_Idle()
{
	while ((P0 & 0x0f) != 0x0f)
		;
}

/********************************************************************************************
 * �������ƣ�Key_Scan()
 * ��    �ܣ�����ɨ��
 * ��ڲ�������
 * ���ڲ�������ֵ
 *********************************************************************************************/
uchar Key_Scan()
{
	if ((P0 & 0x0f) != 0x0f) //�жϰ���
	{
		Delay(4); //��������
		if ((P0 & 0x0f) != 0x0f)
		{

			switch (P0 & 0x0f) //��������ת���ɼ�ֵ
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
 * �������ƣ�Key_Set_Clock( uchar * clock_time )
 * ��    �ܣ������趨ʱ��
 * ��ڲ�����clock_time[6]ָ��
 * ���ڲ�������
 *********************************************************************************************/
void Key_Set_Clock(uchar *clock_time)
{
	uchar *i;
	i = clock_time;
	clock_time++;		 //ָ���
	Lcd_Set_Clock(2, i); //��Ļ��ʾ����ʱ��
	Key_Idle();
	while (1)
	{
		uchar minute;
		minute = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD ת HEX
		switch (Key_Scan())											 //������
		{
		case up_array:
		{
			minute++;
			if (minute == 60)
				minute = 0;
			(*clock_time) = minute / 10 << 4;
			(*clock_time) = (*clock_time) | (minute % 10); // HEX ת BCD
			Lcd_Set_Clock(2, i);						   //��Ļ��ʾ����ʱ��
			Key_Idle();
		}
		break;
		case down_array:
		{
			if (minute == 0)
				minute = 60;
			minute--;
			(*clock_time) = minute / 10 << 4;
			(*clock_time) = (*clock_time) | (minute % 10); // HEX ת BCD
			Lcd_Set_Clock(2, i);						   //��Ļ��ʾ����ʱ��
			Key_Idle();
		}
		break;
			//******************************************************************************************************************************************************
		case function_array: //����ʱ
		{
			clock_time++;		 //ָ��ʱ
			Lcd_Set_Clock(3, i); //��Ļ��ʾ����ʱ��
			Key_Idle();
			while (1)
			{
				uchar hour;
				hour = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD ת HEX
				switch (Key_Scan())
				{
				case up_array:
				{
					hour++;
					if (hour == 24)
						hour = 0;
					(*clock_time) = hour / 10 << 4;
					(*clock_time) = (*clock_time) | (hour % 10); // HEX ת BCD
					Lcd_Set_Clock(3, i);						 //��Ļ��ʾ����ʱ��
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
					(*clock_time) = (*clock_time) | (hour % 10); // HEX ת BCD
					Lcd_Set_Clock(3, i);						 //��Ļ��ʾ����ʱ��
					Key_Idle();
				}
				break;
					//******************************************************************************************************************************************************
				case function_array: //������
				{
					clock_time++;		 //ָ����
					Lcd_Set_Clock(4, i); //��Ļ��ʾ����ʱ��
					Key_Idle();
					while (1)
					{
						uchar day;
						day = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD ת HEX
						switch (Key_Scan())
						{
						case up_array:
						{
							day++;
							if (day == 32)
								day = 1;
							(*clock_time) = day / 10 << 4;
							(*clock_time) = (*clock_time) | (day % 10); // HEX ת BCD
							Lcd_Set_Clock(4, i);						//��Ļ��ʾ����ʱ��
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
							(*clock_time) = (*clock_time) | (day % 10); // HEX ת BCD ;
							Lcd_Set_Clock(4, i);						//��Ļ��ʾ����ʱ��
							Key_Idle();
						}
						break;
							//******************************************************************************************************************************************************
						case function_array: //������
						{
							clock_time++;		 //ָ����
							Lcd_Set_Clock(5, i); //��Ļ��ʾ����ʱ��
							Key_Idle();
							while (1)
							{
								uchar month;
								month = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD ת HEX
								switch (Key_Scan())
								{
								case up_array:
								{
									month++;
									if (month == 13)
										month = 1;
									(*clock_time) = month / 10 << 4;
									(*clock_time) = (*clock_time) | (month % 10); // HEX ת BCD
									Lcd_Set_Clock(5, i);						  //��Ļ��ʾ����ʱ��
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
									(*clock_time) = (*clock_time) | (month % 10); // HEX ת BCD
									Lcd_Set_Clock(5, i);						  //��Ļ��ʾ����ʱ��
									Key_Idle();
								}
								break;
									//******************************************************************************************************************************************************
								case function_array: //������
								{
									clock_time++;		 //ָ����
									Lcd_Set_Clock(6, i); //��Ļ��ʾ����ʱ��
									Key_Idle();
									while (1)
									{
										uchar year;
										year = ((*clock_time >> 4) * 10) + ((*clock_time) & 0x0f); // BCD ת HEX
										switch (Key_Scan())
										{
										case up_array:
										{
											year++;
											if (year == 100)
												year = 0;
											(*clock_time) = year / 10 << 4;
											(*clock_time) = (*clock_time) | (year % 10); // HEX ת BCD
											Lcd_Set_Clock(6, i);						 //��Ļ��ʾ����ʱ��
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
											(*clock_time) = (*clock_time) | (year % 10); // HEX ת BCD
											Lcd_Set_Clock(6, i);						 //��Ļ��ʾ����ʱ��
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
 * ����: Key_Set_Alarm( uchar *  alarm_time )
 * ����: ���ӵ���
 * ��ڲ���: ���ӵ�ַ
 * ���ڲ���:
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
 * ����: Key_Function( uchar * clock_time )
 * ����: ϵͳ���ܵ���
 * ��ڲ���: ʱ�ӳ���
 * ���ڲ���: ��
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
/****************************�ļ�����*************************************/
#include < reg52.h >
#include < character.h >
#include < lcd.h >
#include < clock.h >
#include < sensor.h>
#include < calendar.h >
#include < key.h >

/*****************************Ԥ����**************************************/
#define uchar unsigned char
#define uint unsigned int
/****************************************************************************/
sbit bell = P2 ^ 0; //����������˿�
/*****************************************************************************
 * ����: Timer0_Service() inturrupt 1
 * ����: �жϷ������ ���㱨ʱ 3���ཱུ�����
 * ��ڲ���:
 * ���ڲ���:
 *****************************************************************************/
void Timer0_Service() interrupt 1
{
	static uchar count = 0;
	static uchar flag = 0; //��¼���еĴ���
	count = 0;
	TR0 = 0; //�ر�Timer0
	TH0 = 0x3c;
	TL0 = 0XB0; //��ʱ 50 ms
	TR0 = 1;	//����Timer0
	count++;
	if (count == 20) //���� 1 ��
	{
		bell = ~bell;
		count = 0;
		flag++;
	}
	if (flag == 6)
	{
		flag = 0;
		TR0 = 0; //�ر�Timer0
	}
}
/*****************************************************************************
 * ����: Timer2_Servie() interrupt 5
 * ����: �жϷ������  ���㱨ʱ һ����
 * ��ڲ���:
 * ���ڲ���:
 *****************************************************************************/
uchar HexNum_Convert(uchar HexNum) /*ʱ��洢��λ��ʮλ�ķ�ʽ�������õ�ʮ���Ʋ�һ�����㶮�ģ��Ǻ�--BB��*/
{
	uchar Numtemp;
	Numtemp = (HexNum >> 4) * 10 + (HexNum & 0X0F);
	return Numtemp;
}
/******************************************************************************
 * �������ƣ�main()
 * ��    �ܣ�
 * ��ڲ�����
 * ���ڲ�����
 ********************************************************************************/
void main(void)
{
	uchar clock_time[6] = {0X00, 0X59, 0X23, 0X09, 0X04, 0X11}; //����ʱ����� ��  �� ʱ �� �� ��
	uchar alarm_time[2] = {10, 06};								//��������  alarm_time[0]: ����  alarm_time[1] :Сʱ
	uchar temperature[2];										//�����¶ȱ���   temperature[0]  ��8λ   temperature[1]  ��8λ
	Lcd_Initial();												// LCD��ʼ��
	Clock_Fresh(clock_time);									//�Ұ�ʱ��ˢ�·������proteus����õ�ǰϵͳʱ�䣬����Ϊʲô�����飬�Ǻ�
	Clock_Initial(clock_time);									//ʱ�ӳ��Ի�

	/***********************�жϳ�ʼ��***************************/
	EA = 1;		 //�����ж�
	ET0 = 1;	 // Timer0 ���ж�
	ET2 = 1;	 // Timer2 ���ж�
	TMOD = 0x01; // Timer0 ������ʽ 1
	RCAP2H = 0x3c;
	RCAP2L = 0xb0; // Timer2 ��ʱ 50 ms

	while (1)
	{
		switch (Key_Scan())
		{
		case up_array:
		{
			Key_Idle();
		}
		break;
		case down_array:
		{
			Key_Idle();
		}
		break;
		case clear_array:
		{
			Key_Idle();
		}
		break;
		case function_array:
		{
			Key_Function(clock_time, alarm_time);
		}
		case null:
		{
			Clock_Fresh(clock_time); //ʱ��ˢ��
			Lcd_Clock(clock_time);	 //ʱ����ʾ

			Sensor_Fresh(temperature);	 //�¶ȸ���
			Lcd_Temperture(temperature); //�¶���ʾ

			Calendar_Convert(0, clock_time);
			Week_Convert(0, clock_time);

			//���㱨ʱ
			if ((*clock_time == 0x59) && (*(clock_time + 1) == 0x59))
			{
				bell = 0;
				TR2 = 1; //����Timer2
			}
			//���ӱ���
			if (*alarm_time == HexNum_Convert(*(clock_time + 1)))			//�������Ǻ�
				if (*(alarm_time + 1) == HexNum_Convert(*(clock_time + 2))) //Сʱ���Ǻ�
				{
					bell = 0;
					TR2 = 1; //����Timer2
				}
		}
		break;
		}
	}
}

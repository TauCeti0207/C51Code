#include <REGX52.H>
#include <INTRINS.H>
#include "main.h"
#include "DelayXms.h"
#include "LCD1602.h"
#include "HX711.h"
#include "MatrixKey.h"
#include "Key.h"
#include "Buzzer.h"
#include "DS1302.h"
#include "OneWire.h"
#include "DS18B20.h"
unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0, Weight_Shiwu = 0;
char Price_Count              = 0;
unsigned char KEY_NUM         = 0;
unsigned char Price_Buffer[3] = {0x00, 0x00, 0x00};
unsigned long Money           = 0;
bit Flag_OK                   = 0;

float T, TShow;
char TLow, THigh;
unsigned char KeyNum2;

void TemperatureMoudle()
{
    DS18B20_ConvertT(); // �ϵ���ת��һ���¶ȣ���ֹ��һ�ζ����ݴ���
    Delay(1000);        // �ȴ�ת�����
    THigh = 25;
    // THigh = AT24C02_ReadByte(0); //��ȡ�¶���ֵ����
    // TLow = AT24C02_ReadByte(1);
    TLow = 15;
    if (THigh > 125 || TLow < -55 || THigh <= TLow) {
        THigh = 25; // �����ֵ�Ƿ�������ΪĬ��ֵ
        TLow  = 15;
    }
    LCD_Init();
    LCD_ShowString(1, 1, "T:");
    LCD_ShowString(2, 1, "TH:");
    LCD_ShowString(2, 9, "TL:");
    LCD_ShowSignedNum(2, 4, THigh, 3);
    LCD_ShowSignedNum(2, 12, TLow, 3);

    while (1) {
        KeyNum2 = Key();

        /*�¶ȶ�ȡ����ʾ*/
        DS18B20_ConvertT();  // ת���¶�
        T = DS18B20_ReadT(); // ��ȡ�¶�
        if (T < 0)           // ����¶�С��0
        {
            LCD_ShowChar(1, 3, '-'); // ��ʾ����
            TShow = -T;              // ���¶ȱ�Ϊ����
        } else                       // ����¶ȴ��ڵ���0
        {
            LCD_ShowChar(1, 3, '+'); // ��ʾ����
            TShow = T;
        }
        LCD_ShowNum(1, 4, TShow, 3);                              // ��ʾ�¶���������
        LCD_ShowChar(1, 7, '.');                                  // ��ʾС����
        LCD_ShowNum(1, 8, (unsigned long)(TShow * 100) % 100, 2); // ��ʾ�¶�С������

        /*��ֵ�жϼ���ʾ*/
        if (KeyNum2) {
            if (KeyNum2 == 1) // K1������THigh����
            {
                THigh++;
                if (THigh > 125) {
                    THigh = 125;
                }
            }
            if (KeyNum2 == 2) // K2������THigh�Լ�
            {
                THigh--;
                if (THigh <= TLow) {
                    THigh++;
                }
            }
            if (KeyNum2 == 3) // K3������TLow����
            {
                TLow++;
                if (TLow >= THigh) {
                    TLow--;
                }
            }
            if (KeyNum2 == 4) // K4������TLow�Լ�
            {
                TLow--;
                if (TLow < -55) {
                    TLow = -55;
                }
            }
            LCD_ShowSignedNum(2, 4, THigh, 3); // ��ʾ��ֵ����
            LCD_ShowSignedNum(2, 12, TLow, 3);
            // AT24C02_WriteByte(0, THigh); // д�뵽AT24C02�б���
            Delay(5);
            // AT24C02_WriteByte(1, TLow);
            Delay(5);
        }

        // ���»���¾��ᱨ��
        if (T > THigh) // Խ���ж�
        {
            // Buzzer_Time(500);
            LCD_ShowString(1, 13, "OV:H");
        } else if (T < TLow) {
            // Buzzer_Time(500);
            LCD_ShowString(1, 13, "OV:L");
        } else {
            LCD_ShowString(1, 13, "    ");
        }
    }
}

unsigned char KeyNum, MODE, TimeSetSelect, TimeSetFlashFlag;

void TimeShow(void) // ʱ����ʾ����
{
    DS1302_ReadTime();                    // ��ȡʱ��
    LCD_ShowNum(1, 1, DS1302_Time[0], 2); // ��ʾ��
    LCD_ShowNum(1, 4, DS1302_Time[1], 2); // ��ʾ��
    LCD_ShowNum(1, 7, DS1302_Time[2], 2); // ��ʾ��
    LCD_ShowNum(2, 1, DS1302_Time[3], 2); // ��ʾʱ
    LCD_ShowNum(2, 4, DS1302_Time[4], 2); // ��ʾ��
    LCD_ShowNum(2, 7, DS1302_Time[5], 2); // ��ʾ��
}

void TimeSet(void) // ʱ�����ù���
{
    if (KeyNum == 2) // ����2����
    {
        TimeSetSelect++;    // ����ѡ��λ��1
        TimeSetSelect %= 6; // Խ������
    }
    if (KeyNum == 3) // ����3����
    {
        DS1302_Time[TimeSetSelect]++; // ʱ������λ��ֵ��1
        if (DS1302_Time[0] > 99) {
            DS1302_Time[0] = 0;
        } // ��Խ���ж�
        if (DS1302_Time[1] > 12) {
            DS1302_Time[1] = 1;
        } // ��Խ���ж�
        if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
            DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) // ��Խ���ж�
        {
            if (DS1302_Time[2] > 31) {
                DS1302_Time[2] = 1;
            } // ����
        } else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11) {
            if (DS1302_Time[2] > 30) {
                DS1302_Time[2] = 1;
            } // С��
        } else if (DS1302_Time[1] == 2) {
            if (DS1302_Time[0] % 4 == 0) // �ж�����
            {
                if (DS1302_Time[2] > 29) {
                    DS1302_Time[2] = 1;
                } // ����2��
            } else {
                if (DS1302_Time[2] > 28) {
                    DS1302_Time[2] = 1;
                } // ƽ��2��
            }
        }
        if (DS1302_Time[3] > 23) {
            DS1302_Time[3] = 0;
        } // ʱԽ���ж�
        if (DS1302_Time[4] > 59) {
            DS1302_Time[4] = 0;
        } // ��Խ���ж�
        if (DS1302_Time[5] > 59) {
            DS1302_Time[5] = 0;
        } // ��Խ���ж�
    }
    if (KeyNum == 4) // ����3����
    {
        DS1302_Time[TimeSetSelect]--; // ʱ������λ��ֵ��1
        if (DS1302_Time[0] < 0) {
            DS1302_Time[0] = 99;
        } // ��Խ���ж�
        if (DS1302_Time[1] < 1) {
            DS1302_Time[1] = 12;
        } // ��Խ���ж�
        if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
            DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) // ��Խ���ж�
        {
            if (DS1302_Time[2] < 1) {
                DS1302_Time[2] = 31;
            } // ����
            if (DS1302_Time[2] > 31) {
                DS1302_Time[2] = 1;
            }
        } else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11) {
            if (DS1302_Time[2] < 1) {
                DS1302_Time[2] = 30;
            } // С��
            if (DS1302_Time[2] > 30) {
                DS1302_Time[2] = 1;
            }
        } else if (DS1302_Time[1] == 2) {
            if (DS1302_Time[0] % 4 == 0) {
                if (DS1302_Time[2] < 1) {
                    DS1302_Time[2] = 29;
                } // ����2��
                if (DS1302_Time[2] > 29) {
                    DS1302_Time[2] = 1;
                }
            } else {
                if (DS1302_Time[2] < 1) {
                    DS1302_Time[2] = 28;
                } // ƽ��2��
                if (DS1302_Time[2] > 28) {
                    DS1302_Time[2] = 1;
                }
            }
        }
        if (DS1302_Time[3] < 0) {
            DS1302_Time[3] = 23;
        } // ʱԽ���ж�
        if (DS1302_Time[4] < 0) {
            DS1302_Time[4] = 59;
        } // ��Խ���ж�
        if (DS1302_Time[5] < 0) {
            DS1302_Time[5] = 59;
        } // ��Խ���ж�
    }

    // ������֮�������ʾ������TimeSetSelect��TimeSetFlashFlag�жϿ������˸����
    if (TimeSetSelect == 0 && TimeSetFlashFlag == 1) {
        LCD_ShowString(1, 1, "  ");
    } else {
        LCD_ShowNum(1, 1, DS1302_Time[0], 2);
    }
    if (TimeSetSelect == 1 && TimeSetFlashFlag == 1) {
        LCD_ShowString(1, 4, "  ");
    } else {
        LCD_ShowNum(1, 4, DS1302_Time[1], 2);
    }
    if (TimeSetSelect == 2 && TimeSetFlashFlag == 1) {
        LCD_ShowString(1, 7, "  ");
    } else {
        LCD_ShowNum(1, 7, DS1302_Time[2], 2);
    }
    if (TimeSetSelect == 3 && TimeSetFlashFlag == 1) {
        LCD_ShowString(2, 1, "  ");
    } else {
        LCD_ShowNum(2, 1, DS1302_Time[3], 2);
    }
    if (TimeSetSelect == 4 && TimeSetFlashFlag == 1) {
        LCD_ShowString(2, 4, "  ");
    } else {
        LCD_ShowNum(2, 4, DS1302_Time[4], 2);
    }
    if (TimeSetSelect == 5 && TimeSetFlashFlag == 1) {
        LCD_ShowString(2, 7, "  ");
    } else {
        LCD_ShowNum(2, 7, DS1302_Time[5], 2);
    }
}

void TimeMoudle()
{
    LCD_Init();
    DS1302_Init();
    // Timer0Init();
    LCD_ShowString(1, 1, "  -  -  "); // ��̬�ַ���ʼ����ʾ
    LCD_ShowString(2, 1, "  :  :  ");
    DS1302_SetTime(); // ����ʱ��

    while (1) {
        if (DS1302_Time[4] == 59 && DS1302_Time[5] == 59) {
            Buzzer_Time(1000); // ���㱨ʱ1s
        }
        TimeShow();
    }
}

// ������

void main()
{
    Init_LCD1602(); // ��ʼ��LCD1602

    LCD1602_write_com(0x80);             // ָ������
    LCD1602_write_word("yangzhiqiang "); // ���������һ��
    LCD1602_write_com(0x80 + 0x40);      // ָ������
    LCD1602_write_word("2220201357");

    Delay(5000); // ��ʱ

loop:
    Price_Count     = 0;
    Price_Buffer[0] = 0;
    Price_Buffer[1] = 0;
    Price_Buffer[2] = 0;
    Flag_OK         = 0;

    LCD1602_write_com(0x80); // ָ������
    LCD1602_write_word("WEI - PRI - MON ");
    LCD1602_write_com(0x80 + 0x40); // ָ������
    LCD1602_write_word("0.000-  . -   . ");

    Get_Maopi(); // ��ëƤ����

    while (1) {
        if (Flag_OK == 0) {
            Get_Weight(); // ����
            Delay(10);
            // ��ʾ��ǰ����
            LCD1602_write_com(0x80 + 0x40);
            LCD1602_write_data(Weight_Shiwu / 1000 + 0x30);
            LCD1602_write_data('.');
            LCD1602_write_data(Weight_Shiwu % 1000 / 100 + 0x30);
            LCD1602_write_data(Weight_Shiwu % 100 / 10 + 0x30);
            LCD1602_write_data(Weight_Shiwu % 10 + 0x30);
        }

        KEY_NUM = MatrixKey();

        if (KEY_NUM != 0x55) // �����صĲ��ǳ�ֵʱ��ȷ�ϰ������¡�
        {
            if (KEY_NUM == 10) // ����10����ȥƤ����
            {
                Get_Maopi(); // ȥƤ
            }

            if (KEY_NUM == 11) // ����11������������β���
            {
                goto loop;
            }

            if (KEY_NUM == 12) // ����C���뵥�۴���ʱ������һ��
            {
                Price_Count--;
                if (Price_Count < 0) {
                    Price_Count = 0;
                }

                Price_Buffer[Price_Count] = 0; // �����һ�����������

                switch (Price_Count) {
                    case 0:
                        LCD1602_write_com(0x80 + 0x40 + 6);
                        LCD1602_write_data(' ');
                        break;
                    case 1:
                        LCD1602_write_com(0x80 + 0x40 + 7);
                        LCD1602_write_data(' ');
                        break;
                    case 2:
                        LCD1602_write_com(0x80 + 0x40 + 9);
                        LCD1602_write_data(' ');
                        break;
                    default:
                        break;
                }
            }

            if (KEY_NUM == 13) {
                TemperatureMoudle();
                // LCD_ShowString(1,1,"123");
            }

            if (KEY_NUM == 14) {
                TimeMoudle();
            }

            if (KEY_NUM == 15) // ���� 10s��ر�LCD
            {
                Delay(10000);
                LCD1602_write_com(0x08); // ��ʾ�ر�
            }

            if (KEY_NUM == 16) // ����16���������ܼ�
            {
                Money = Price_Buffer[0] * 100 + Price_Buffer[1] * 10 + Price_Buffer[2];
                Money = Money * Weight_Shiwu / 1000;
                LCD1602_write_com(0x80 + 0x40 + 11);
                LCD1602_write_data(Money / 1000 + 0x30);
                LCD1602_write_data(Money % 1000 / 100 + 0x30);
                LCD1602_write_data(Money % 100 / 10 + 0x30);
                LCD1602_write_data('.');
                LCD1602_write_data(Money % 10 + 0x30);

                Flag_OK = 1;
            }

            if (KEY_NUM >= 1 && KEY_NUM <= 9) // ��ʾ����ļ۸�ֵ
            {
                Price_Buffer[Price_Count] = KEY_NUM;

                switch (Price_Count) {
                    case 0:
                        LCD1602_write_com(0x80 + 0x40 + 6);
                        LCD1602_write_data(Price_Buffer[0] + 0x30);
                        break;
                    case 1:
                        LCD1602_write_com(0x80 + 0x40 + 7);
                        LCD1602_write_data(Price_Buffer[1] + 0x30);
                        break;
                    case 2:
                        LCD1602_write_com(0x80 + 0x40 + 9);
                        LCD1602_write_data(Price_Buffer[2] + 0x30);
                        break;
                    default:
                        break;
                }

                Price_Count++;

                if (Price_Count >= 3) {
                    Price_Count = 3;
                }
            }
        }
    }
}

//****************************************************
// ����
//****************************************************
void Get_Weight()
{
    HX711_Buffer = HX711_Read();
    HX711_Buffer = HX711_Buffer / 100;
    if (HX711_Buffer > Weight_Maopi) {
        Weight_Shiwu = HX711_Buffer;
        Weight_Shiwu = Weight_Shiwu - Weight_Maopi; // ��ȡʵ���AD������ֵ��

        Weight_Shiwu = (unsigned int)((float)Weight_Shiwu / 4.22 + 0.05); // ����ʵ���ʵ������
                                                                          // ��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������4.30���������
                                                                          // �����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
                                                                          // ������Գ���������ƫСʱ����С����ֵ��
                                                                          // ����ֵһ����4.0-5.0֮�䡣�򴫸�����ͬ������
                                                                          //+0.05��Ϊ����������ٷ�λ
    }

    if (Weight_Shiwu > 0 && Weight_Shiwu <= 200) {
        Weight_Shiwu = 20.1469 * Weight_Shiwu - 0.1175 - 0.9;
    } else if (Weight_Shiwu > 200 && Weight_Shiwu <= 600) {
        Weight_Shiwu = 20.1469 * Weight_Shiwu - 0.1175 - 1.3;
    } else if (Weight_Shiwu > 600 && Weight_Shiwu <= 1000) {
        Weight_Shiwu = 20.1469 * Weight_Shiwu - 0.1175 - 2.1;
    } else if (Weight_Shiwu > 1000 && Weight_Shiwu <= 3000) {
        Weight_Shiwu = 20.1469 * Weight_Shiwu - 0.1175 - 4.4;
    } else if (Weight_Shiwu > 3000 && Weight_Shiwu <= 5000) {
        Weight_Shiwu = 20.1469 * Weight_Shiwu - 0.1175 - 6.2;
    } else if (Weight_Shiwu > 5000 || HX711_Buffer < Weight_Maopi - 30) {
        Buzzer_Time(200);
        // Weight_Shiwu = 0;
    }
}

//****************************************************
// ��ȡëƤ����
//****************************************************
void Get_Maopi()
{
    HX711_Buffer = HX711_Read();
    Weight_Maopi = HX711_Buffer / 100;
}

//****************************************************
// MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
    unsigned int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 123; j++)
            ;
}
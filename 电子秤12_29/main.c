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
    DS18B20_ConvertT(); // 上电先转换一次温度，防止第一次读数据错误
    Delay(1000);        // 等待转换完成
    THigh = 25;
    // THigh = AT24C02_ReadByte(0); //读取温度阈值数据
    // TLow = AT24C02_ReadByte(1);
    TLow = 15;
    if (THigh > 125 || TLow < -55 || THigh <= TLow) {
        THigh = 25; // 如果阈值非法，则设为默认值
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

        /*温度读取及显示*/
        DS18B20_ConvertT();  // 转换温度
        T = DS18B20_ReadT(); // 读取温度
        if (T < 0)           // 如果温度小于0
        {
            LCD_ShowChar(1, 3, '-'); // 显示负号
            TShow = -T;              // 将温度变为正数
        } else                       // 如果温度大于等于0
        {
            LCD_ShowChar(1, 3, '+'); // 显示正号
            TShow = T;
        }
        LCD_ShowNum(1, 4, TShow, 3);                              // 显示温度整数部分
        LCD_ShowChar(1, 7, '.');                                  // 显示小数点
        LCD_ShowNum(1, 8, (unsigned long)(TShow * 100) % 100, 2); // 显示温度小数部分

        /*阈值判断及显示*/
        if (KeyNum2) {
            if (KeyNum2 == 1) // K1按键，THigh自增
            {
                THigh++;
                if (THigh > 125) {
                    THigh = 125;
                }
            }
            if (KeyNum2 == 2) // K2按键，THigh自减
            {
                THigh--;
                if (THigh <= TLow) {
                    THigh++;
                }
            }
            if (KeyNum2 == 3) // K3按键，TLow自增
            {
                TLow++;
                if (TLow >= THigh) {
                    TLow--;
                }
            }
            if (KeyNum2 == 4) // K4按键，TLow自减
            {
                TLow--;
                if (TLow < -55) {
                    TLow = -55;
                }
            }
            LCD_ShowSignedNum(2, 4, THigh, 3); // 显示阈值数据
            LCD_ShowSignedNum(2, 12, TLow, 3);
            // AT24C02_WriteByte(0, THigh); // 写入到AT24C02中保存
            Delay(5);
            // AT24C02_WriteByte(1, TLow);
            Delay(5);
        }

        // 高温或低温均会报警
        if (T > THigh) // 越界判断
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

void TimeShow(void) // 时间显示功能
{
    DS1302_ReadTime();                    // 读取时间
    LCD_ShowNum(1, 1, DS1302_Time[0], 2); // 显示年
    LCD_ShowNum(1, 4, DS1302_Time[1], 2); // 显示月
    LCD_ShowNum(1, 7, DS1302_Time[2], 2); // 显示日
    LCD_ShowNum(2, 1, DS1302_Time[3], 2); // 显示时
    LCD_ShowNum(2, 4, DS1302_Time[4], 2); // 显示分
    LCD_ShowNum(2, 7, DS1302_Time[5], 2); // 显示秒
}

void TimeSet(void) // 时间设置功能
{
    if (KeyNum == 2) // 按键2按下
    {
        TimeSetSelect++;    // 设置选择位加1
        TimeSetSelect %= 6; // 越界清零
    }
    if (KeyNum == 3) // 按键3按下
    {
        DS1302_Time[TimeSetSelect]++; // 时间设置位数值加1
        if (DS1302_Time[0] > 99) {
            DS1302_Time[0] = 0;
        } // 年越界判断
        if (DS1302_Time[1] > 12) {
            DS1302_Time[1] = 1;
        } // 月越界判断
        if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
            DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) // 日越界判断
        {
            if (DS1302_Time[2] > 31) {
                DS1302_Time[2] = 1;
            } // 大月
        } else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11) {
            if (DS1302_Time[2] > 30) {
                DS1302_Time[2] = 1;
            } // 小月
        } else if (DS1302_Time[1] == 2) {
            if (DS1302_Time[0] % 4 == 0) // 判断闰年
            {
                if (DS1302_Time[2] > 29) {
                    DS1302_Time[2] = 1;
                } // 闰年2月
            } else {
                if (DS1302_Time[2] > 28) {
                    DS1302_Time[2] = 1;
                } // 平年2月
            }
        }
        if (DS1302_Time[3] > 23) {
            DS1302_Time[3] = 0;
        } // 时越界判断
        if (DS1302_Time[4] > 59) {
            DS1302_Time[4] = 0;
        } // 分越界判断
        if (DS1302_Time[5] > 59) {
            DS1302_Time[5] = 0;
        } // 秒越界判断
    }
    if (KeyNum == 4) // 按键3按下
    {
        DS1302_Time[TimeSetSelect]--; // 时间设置位数值减1
        if (DS1302_Time[0] < 0) {
            DS1302_Time[0] = 99;
        } // 年越界判断
        if (DS1302_Time[1] < 1) {
            DS1302_Time[1] = 12;
        } // 月越界判断
        if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
            DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12) // 日越界判断
        {
            if (DS1302_Time[2] < 1) {
                DS1302_Time[2] = 31;
            } // 大月
            if (DS1302_Time[2] > 31) {
                DS1302_Time[2] = 1;
            }
        } else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11) {
            if (DS1302_Time[2] < 1) {
                DS1302_Time[2] = 30;
            } // 小月
            if (DS1302_Time[2] > 30) {
                DS1302_Time[2] = 1;
            }
        } else if (DS1302_Time[1] == 2) {
            if (DS1302_Time[0] % 4 == 0) {
                if (DS1302_Time[2] < 1) {
                    DS1302_Time[2] = 29;
                } // 闰年2月
                if (DS1302_Time[2] > 29) {
                    DS1302_Time[2] = 1;
                }
            } else {
                if (DS1302_Time[2] < 1) {
                    DS1302_Time[2] = 28;
                } // 平年2月
                if (DS1302_Time[2] > 28) {
                    DS1302_Time[2] = 1;
                }
            }
        }
        if (DS1302_Time[3] < 0) {
            DS1302_Time[3] = 23;
        } // 时越界判断
        if (DS1302_Time[4] < 0) {
            DS1302_Time[4] = 59;
        } // 分越界判断
        if (DS1302_Time[5] < 0) {
            DS1302_Time[5] = 59;
        } // 秒越界判断
    }

    // 设置完之后更新显示，根据TimeSetSelect和TimeSetFlashFlag判断可完成闪烁功能
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
    LCD_ShowString(1, 1, "  -  -  "); // 静态字符初始化显示
    LCD_ShowString(2, 1, "  :  :  ");
    DS1302_SetTime(); // 设置时间

    while (1) {
        if (DS1302_Time[4] == 59 && DS1302_Time[5] == 59) {
            Buzzer_Time(1000); // 整点报时1s
        }
        TimeShow();
    }
}

// 主函数

void main()
{
    Init_LCD1602(); // 初始化LCD1602

    LCD1602_write_com(0x80);             // 指针设置
    LCD1602_write_word("yangzhiqiang "); // 开机画面第一行
    LCD1602_write_com(0x80 + 0x40);      // 指针设置
    LCD1602_write_word("2220201357");

    Delay(5000); // 延时

loop:
    Price_Count     = 0;
    Price_Buffer[0] = 0;
    Price_Buffer[1] = 0;
    Price_Buffer[2] = 0;
    Flag_OK         = 0;

    LCD1602_write_com(0x80); // 指针设置
    LCD1602_write_word("WEI - PRI - MON ");
    LCD1602_write_com(0x80 + 0x40); // 指针设置
    LCD1602_write_word("0.000-  . -   . ");

    Get_Maopi(); // 称毛皮重量

    while (1) {
        if (Flag_OK == 0) {
            Get_Weight(); // 称重
            Delay(10);
            // 显示当前重量
            LCD1602_write_com(0x80 + 0x40);
            LCD1602_write_data(Weight_Shiwu / 1000 + 0x30);
            LCD1602_write_data('.');
            LCD1602_write_data(Weight_Shiwu % 1000 / 100 + 0x30);
            LCD1602_write_data(Weight_Shiwu % 100 / 10 + 0x30);
            LCD1602_write_data(Weight_Shiwu % 10 + 0x30);
        }

        KEY_NUM = MatrixKey();

        if (KEY_NUM != 0x55) // 当返回的不是初值时候，确认按键按下。
        {
            if (KEY_NUM == 10) // 数字10键，去皮功能
            {
                Get_Maopi(); // 去皮
            }

            if (KEY_NUM == 11) // 数字11键清除键，二次测量
            {
                goto loop;
            }

            if (KEY_NUM == 12) // 数字C输入单价错误时返回上一步
            {
                Price_Count--;
                if (Price_Count < 0) {
                    Price_Count = 0;
                }

                Price_Buffer[Price_Count] = 0; // 清除上一个输入的数据

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

            if (KEY_NUM == 15) // 节能 10s后关闭LCD
            {
                Delay(10000);
                LCD1602_write_com(0x08); // 显示关闭
            }

            if (KEY_NUM == 16) // 数字16键，计算总价
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

            if (KEY_NUM >= 1 && KEY_NUM <= 9) // 显示输入的价格值
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
// 称重
//****************************************************
void Get_Weight()
{
    HX711_Buffer = HX711_Read();
    HX711_Buffer = HX711_Buffer / 100;
    if (HX711_Buffer > Weight_Maopi) {
        Weight_Shiwu = HX711_Buffer;
        Weight_Shiwu = Weight_Shiwu - Weight_Maopi; // 获取实物的AD采样数值。

        Weight_Shiwu = (unsigned int)((float)Weight_Shiwu / 4.22 + 0.05); // 计算实物的实际重量
                                                                          // 因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的4.30这个除数。
                                                                          // 当发现测试出来的重量偏大时，增加该数值。
                                                                          // 如果测试出来的重量偏小时，减小改数值。
                                                                          // 该数值一般在4.0-5.0之间。因传感器不同而定。
                                                                          //+0.05是为了四舍五入百分位
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
// 获取毛皮重量
//****************************************************
void Get_Maopi()
{
    HX711_Buffer = HX711_Read();
    Weight_Maopi = HX711_Buffer / 100;
}

//****************************************************
// MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
    unsigned int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 123; j++)
            ;
}
//2220201357 ÑîÖ¾Ç¿
#include "reg51.h"
#define uchar unsigned char
unsigned char key();
unsigned char code SMG_Table[12] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,
                            0x7D,0x07,0x7F,0x6F,0x76,0x37};
unsigned char code Key_Table[12] = {0xDE,0XB7,0XD7,0XE7,0XBB,0XDB,0XEB,0XBD,0XDD,0XED,0XEE,0XBE};

sbit Key_Port = P1;

void delay(int aa)
{
    for(;aa>0;aa--);
}

void main()
{
    uchar i;
    while(1)
    {
        Key_Port = 0XFF;
        for(i = 0;i < 12;i++)
        {
            if(Key_Port == Key_Table[i])
            {
                delay(200);
                if(Key_Port == Key_Table[i])
                {
                    key();
                }
            }
        }
    }
}
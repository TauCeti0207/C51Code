//#include<reg51.h>
//unsigned char i,j,k;
//void main()
//{
//    TMOD=0x01; // GATE=0 C/T=0 M1M0=01 定时器0，方式1
//    TR0=1; // 启动计数
//    while(1)
//    {
//        P1=0x00;
//        for(i=0;i<10;i++)
//        {
//            for(j=0;j<10;j++)
//            {
//                for(k=0;k<20;k++)  // 延时1s 20*50ms=1s
//                {
//                    TH0=15536/256;
//                    TL0=15536%256;
//                    while(!TF0);
//                    TF0=0;
//                }
//                P1=P1+1; //每10次进1
//            }
//            P1=(P1+0X10)&0XF0; //进位并将个位置0
//        }
//    }
//}	

#include<reg51.h>
#include<intrins.h>
void timer1()interrupt 3
{
    unsigned char i,j,k;
    for(k = 0; k < 10; ++k)
    {
        for(i=0;i<10;i++)
        {
            for(j=0;j<20;j++)  // 延时50ms*20 = 1s
            {
                TH0=15536/256;
                TL0=15536%256;
                while(!TF0);
                TF0=0; //硬件自动清零，不写也行
            }
            P1=P1+1;
        }
        P1 = (P1+0x10) & 0xF0;
    }
}

void main()
{
    TMOD=0x01; // 定时器0，方式1
    TH0=15536/256;
    TL0=15536%256;
    EA=1;
    ET0=1;
    TR0=1;// 启动定时
    while(1); // 等待中断
}   
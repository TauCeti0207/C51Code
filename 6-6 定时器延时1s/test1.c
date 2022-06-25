//#include<reg51.h>
//unsigned char i,j,k;
//void main()
//{
//    TMOD=0x01; // GATE=0 C/T=0 M1M0=01 ��ʱ��0����ʽ1
//    TR0=1; // ��������
//    while(1)
//    {
//        P1=0x00;
//        for(i=0;i<10;i++)
//        {
//            for(j=0;j<10;j++)
//            {
//                for(k=0;k<20;k++)  // ��ʱ1s 20*50ms=1s
//                {
//                    TH0=15536/256;
//                    TL0=15536%256;
//                    while(!TF0);
//                    TF0=0;
//                }
//                P1=P1+1; //ÿ10�ν�1
//            }
//            P1=(P1+0X10)&0XF0; //��λ������λ��0
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
            for(j=0;j<20;j++)  // ��ʱ50ms*20 = 1s
            {
                TH0=15536/256;
                TL0=15536%256;
                while(!TF0);
                TF0=0; //Ӳ���Զ����㣬��дҲ��
            }
            P1=P1+1;
        }
        P1 = (P1+0x10) & 0xF0;
    }
}

void main()
{
    TMOD=0x01; // ��ʱ��0����ʽ1
    TH0=15536/256;
    TL0=15536%256;
    EA=1;
    ET0=1;
    TR0=1;// ������ʱ
    while(1); // �ȴ��ж�
}   
#include <reg51.h>
#define uchar unsigned char 
uchar data BUF1[5]={0x22,0x20,0x20,0x13,0x57};
uchar data BUF2[10];
// 22 20 20 13 57
// 32 32 30 32  30 32 33 31 37 35
void main() //定义主函数，函数无返回类型
{
    uchar i; //定义普通变量x，类型为无符号字节
    for(i=0;i<5;i++) //实现5次循环，10次转换
    {
        BUF2[i*2]=0x30+(BUF1[i]&0x0f);
        BUF2[i*2+1]=0x30+(BUF1[i]>>4);
    }
    while(1); //动态停机，相当于执行指令SJMP $
}
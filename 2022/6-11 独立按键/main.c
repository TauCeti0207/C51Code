#include <REGX52.H> // REGX52有关于位定义的，而REG52则没有，需要自己sbit定义
#include <INTRINS.H>

// 延时xms
void Delay1ms(unsigned int xms) //@11.0592MHz
{
    unsigned char i, j;
    while (xms--) // while里面是延时1ms的代码
    {
        _nop_();
        i = 2;
        j = 199;
        do
        {
            while (--j)
                ;
        } while (--i);
    }
}
void test1() // LED流水灯
{
    while (1)
    {
        if (P3_1 == 0) // 按下按键K1 点亮LED
        {
            P2_0 = 0;
        }
        else
        {
            P2_0 = 1;
        }
    }
}

// 按下按键才点亮，再按一次就熄灭
void test2()
{
			while (1)
    {
				if (P3_1 == 0)
        {
       Delay1ms(20); // 软件延时消抖
            while (P3_1 == 0)
                ;         // 检测松手
            Delay1ms(20); // 延时消抖

            P2_0 = ~P2_0;
        }
    }
}

// 按下按键实现LED执行二进制加法式的点亮
void test3()
{
    unsigned char LEDNum = 0;
    while (1)
    {
        if (P3_1 == 0)
        {
            Delay1ms(20); // 软件延时消抖
            while (P3_1 == 0)
                ;         // 检测松手
            Delay1ms(20); // 延时消抖

            // P2上电默认是置0xFF
            // P2++; // 再++就变成0x00 也就是一按下，LED全点亮

            LEDNum++; // 0000 0001 再取反 就是1111 1110才是我们需要
            P2 = ~LEDNum;
        }
    }
}

// 独立按键控制LED移位
// 所使用开发板D1~D8是从左到右的
// 按下K1右移，按下K2左移
void test4()
{
    unsigned char LEDNum = 0;
    P2 = 0xFE;// 上电时要让D1点亮
    while (1)
    {
        if (P3_1 == 0)
        {
            // 软件消抖
            Delay1ms(20);
            while (P3_1 == 0)
                ; // 检测松手
            Delay1ms(20);

            LEDNum++;
            if (LEDNum >= 8)
		{
			LEDNum = 0;
            }

            // 0x01 << 1 -->  0000 0010 取反 --> 1111 1101
            // 0x01 << 2 -->  0000 0100 取反 --> 1111 1011
            // ...
            // 0x01 << 7 -->  1000 0000 取反 --> 0111 1111
            // 0x01 << 8 -->  0000 0000 取反 --> 1111 1111
            // 因此当左移到8位时，需要重新从0开始移位
            P2 = ~(0x01 << LEDNum);
        }

        // 按下K2右移
        if (P3_0 == 0)
				{ 
            // 软件消抖
            Delay1ms(20);
            while (P3_0 == 0)
                ; // 检测松手
            Delay1ms(20);

            if(LEDNum == 0)
            {
                LEDNum = 7; // 0再--就变成255了
            }
            else
            {
                LEDNum--;
            }

            P2 = ~(0x01 << LEDNum);
        }
    }
}
void main()
{
    test4();
}

#include <REGX52.H>
#include <INTRINS.H>
void test1() // 点亮第一个LED
{
    P2 = 0xFE; // 1111 1110 点亮D1LED
    P2 = 0xFD; // 1111 1101 点亮D2LED
    P2 = 0xFB; // 1111 1011 点亮D3LED
    P2 = 0xF0; // 1111 0000 点亮D1~D4

    P2 = 0x55; // 0101 0101 间隔亮灭
    while (1)  // 动态停机
    {
    }
}
void Delay500ms() //@11.0592MHz
{
    unsigned char i, j, k;

    _nop_();
    i = 4;
    j = 129;
    k = 119;
    do {
        do {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void test2() // LED闪烁
{
    while (1) {
        P2 = 0xFE;
        Delay500ms();
        P2 = 0xFF;
        Delay500ms();
    }
}

void test3() // LED流水灯
{
    P2 = 0xFE;
    while (1) {
        P2 = _crol_(P2, 1); // 循环左移1位 该函数包含在intrins头文件
        Delay500ms();
    }
}

// 延时xms
void Delay1ms(unsigned int xms) //@11.0592MHz
{
    unsigned char i, j;
    while (xms) // while里面是延时1ms的代码
    {
        _nop_();
        i = 2;
        j = 199;
        do {
            while (--j)
                ;
        } while (--i);
        --xms;
    }
}

void test4() // LED流水灯
{
    P2 = 0xFE;
    while (1) {
        P2 = _crol_(P2, 1); // 循环左移1位 该函数包含在intrins头文件
        Delay1ms(100);      // 指定任意延时时间
    }
}

void main()
{
    test4();
}
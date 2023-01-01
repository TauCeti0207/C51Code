#include <REGX52.H>
#include <INTRINS.H>
#include "DelayXms.h"
#include "NixieTube.h"

void test1()
{
    // 位选，让第六个数码管（LED6）导通 也就是Y5=0
    P2_4 = 1;
    P2_3 = 0;
    P2_2 = 1;
    // 由于P2口也控制LED的亮灭，因此会产生引脚冲突，LED6数码管点亮的同时，D4也亮了

    // 段选 0111 1101
    P0 = 0x7D;
    while (1)
    {
        // 动态停机
    }
    whiel();
}



void test2()
{
    // 第一位显示1，然后变成第二位显示2
    NixieTube(1, 1);
    Delay1ms(500);
    NixieTube(2, 2);
    Delay1ms(500);
    while (1)
    {

    }
}

// 动态数码管 显示学号
void test3()
{
    while (1)
    {
        NixieTube(1, 2);
        NixieTube(2, 0);
        NixieTube(3, 2);
        NixieTube(4, 0);
        NixieTube(5, 1);
        NixieTube(6, 3);
        NixieTube(7, 5);
        NixieTube(8, 7);
    }
}

void main()
{
    test3();
}
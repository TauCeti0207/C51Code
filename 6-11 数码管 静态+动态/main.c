#include <REGX52.H>
#include <INTRINS.H>
#include "DelayXms.h"
#include "NixieTube.h"

void test1()
{
    // λѡ���õ���������ܣ�LED6����ͨ Ҳ����Y5=0
    P2_4 = 1;
    P2_3 = 0;
    P2_2 = 1;
    // ����P2��Ҳ����LED��������˻�������ų�ͻ��LED6����ܵ�����ͬʱ��D4Ҳ����

    // ��ѡ 0111 1101
    P0 = 0x7D;
    while (1)
    {
        // ��̬ͣ��
    }
    whiel();
}



void test2()
{
    // ��һλ��ʾ1��Ȼ���ɵڶ�λ��ʾ2
    NixieTube(1, 1);
    Delay1ms(500);
    NixieTube(2, 2);
    Delay1ms(500);
    while (1)
    {

    }
}

// ��̬����� ��ʾѧ��
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
#include <REGX52.H>
#include <INTRINS.H>
void test1() // ������һ��LED
{
    P2 = 0xFE; // 1111 1110 ����D1LED
    P2 = 0xFD; // 1111 1101 ����D2LED
    P2 = 0xFB; // 1111 1011 ����D3LED
    P2 = 0xF0; // 1111 0000 ����D1~D4

    P2 = 0x55; // 0101 0101 �������
    while (1)  // ��̬ͣ��
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
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void test2() //LED��˸
{
    while (1)
    {
        P2 = 0xFE;
        Delay500ms();
        P2 = 0xFF;
        Delay500ms();
    }
}

void test3() // LED��ˮ��
{
    P2 = 0xFE;
    while(1)
    {
        P2 = _crol_(P2, 1); // ѭ������1λ �ú���������intrinsͷ�ļ�
        Delay500ms();
    }
}
void main()
{
    test3();
}
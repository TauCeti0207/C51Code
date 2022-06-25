#include <REGX52.H> // REGX52�й���λ����ģ���REG52��û�У���Ҫ�Լ�sbit����
#include <INTRINS.H>

// ��ʱxms
void Delay1ms(unsigned int xms) //@11.0592MHz
{
    unsigned char i, j;
    while (xms--) // while��������ʱ1ms�Ĵ���
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
void test1() // LED��ˮ��
{
    while (1)
    {
        if (P3_1 == 0) // ���°���K1 ����LED
        {
            P2_0 = 0;
        }
        else
        {
            P2_0 = 1;
        }
    }
}

// ���°����ŵ������ٰ�һ�ξ�Ϩ��
void test2()
{
			while (1)
    {
				if (P3_1 == 0)
        {
       Delay1ms(20); // �����ʱ����
            while (P3_1 == 0)
                ;         // �������
            Delay1ms(20); // ��ʱ����

            P2_0 = ~P2_0;
        }
    }
}

// ���°���ʵ��LEDִ�ж����Ƽӷ�ʽ�ĵ���
void test3()
{
    unsigned char LEDNum = 0;
    while (1)
    {
        if (P3_1 == 0)
        {
            Delay1ms(20); // �����ʱ����
            while (P3_1 == 0)
                ;         // �������
            Delay1ms(20); // ��ʱ����

            // P2�ϵ�Ĭ������0xFF
            // P2++; // ��++�ͱ��0x00 Ҳ����һ���£�LEDȫ����

            LEDNum++; // 0000 0001 ��ȡ�� ����1111 1110����������Ҫ
            P2 = ~LEDNum;
        }
    }
}

// ������������LED��λ
// ��ʹ�ÿ�����D1~D8�Ǵ����ҵ�
// ����K1���ƣ�����K2����
void test4()
{
    unsigned char LEDNum = 0;
    P2 = 0xFE;// �ϵ�ʱҪ��D1����
    while (1)
    {
        if (P3_1 == 0)
        {
            // �������
            Delay1ms(20);
            while (P3_1 == 0)
                ; // �������
            Delay1ms(20);

            LEDNum++;
            if (LEDNum >= 8)
		{
			LEDNum = 0;
            }

            // 0x01 << 1 -->  0000 0010 ȡ�� --> 1111 1101
            // 0x01 << 2 -->  0000 0100 ȡ�� --> 1111 1011
            // ...
            // 0x01 << 7 -->  1000 0000 ȡ�� --> 0111 1111
            // 0x01 << 8 -->  0000 0000 ȡ�� --> 1111 1111
            // ��˵����Ƶ�8λʱ����Ҫ���´�0��ʼ��λ
            P2 = ~(0x01 << LEDNum);
        }

        // ����K2����
        if (P3_0 == 0)
				{ 
            // �������
            Delay1ms(20);
            while (P3_0 == 0)
                ; // �������
            Delay1ms(20);

            if(LEDNum == 0)
            {
                LEDNum = 7; // 0��--�ͱ��255��
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

#ifndef __UART_H__
#define __UART_H__

void UART_Init();
void UART_SendByte(unsigned char Byte);

/**
 * @brief  ���ڳ�ʼ����4800bps@12.000MHz
 * @param  ��
 * @retval ��
 */
void UART_Init()
{
    SCON = 0x50; // 1001 0000
    PCON |= 0x80;
    TMOD &= 0x0F; //���ö�ʱ��ģʽ
    TMOD |= 0x20; //���ö�ʱ��ģʽ
    TL1 = 0xF4;   //�趨��ʱ��ֵ
    TH1 = 0xF4;   //�趨��ʱ����װֵ
    ET1 = 0;      //��ֹ��ʱ��1�ж�
    TR1 = 1;      //������ʱ��1

    // ������λ����������Ҫ���ж�
    EA = 1; // ���ж�����λ
    ES = 1; // ���п�1�ж�����λ
}

/**
 * @brief  ���ڷ���һ���ֽ�����
 * @param  Byte Ҫ���͵�һ���ֽ�����
 * @retval ��
 */
void UART_SendByte(unsigned char Byte)
{
    SBUF = Byte;
    while (TI == 0)
        ;
    TI = 0;
}

/* �����жϺ���ģ�壬�õ�ʱ��Ų��������
void UART_Routine() interrupt 4
{
    // ����Ƿ��ǽ����ж�
    if (RI == 1)
    {
        P2 = ~SBUF;
        UART_SendByte(SBUF); // ����Է��͵�Ƭ�����յ�������
        RI = 0;              // �����������
    }
}
*/

#endif

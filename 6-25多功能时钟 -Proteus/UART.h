#ifndef __UART_H__
#define __UART_H__

void UART_Init();
void UART_SendByte(unsigned char Byte);

/**
 * @brief  串口初始化，4800bps@12.000MHz
 * @param  无
 * @retval 无
 */
void UART_Init()
{
    SCON = 0x50; // 1001 0000
    PCON |= 0x80;
    TMOD &= 0x0F; //设置定时器模式
    TMOD |= 0x20; //设置定时器模式
    TL1 = 0xF4;   //设定定时初值
    TH1 = 0xF4;   //设定定时器重装值
    ET1 = 0;      //禁止定时器1中断
    TR1 = 1;      //启动定时器1

    // 接收上位机的数据需要靠中断
    EA = 1; // 总中断允许位
    ES = 1; // 串行口1中断允许位
}

/**
 * @brief  串口发送一个字节数据
 * @param  Byte 要发送的一个字节数据
 * @retval 无
 */
void UART_SendByte(unsigned char Byte)
{
    SBUF = Byte;
    while (TI == 0)
        ;
    TI = 0;
}

/* 串口中断函数模板，用的时候挪到主函数
void UART_Routine() interrupt 4
{
    // 检测是否是接收中断
    if (RI == 1)
    {
        P2 = ~SBUF;
        UART_SendByte(SBUF); // 向电脑发送单片机接收到的数据
        RI = 0;              // 必须软件清零
    }
}
*/

#endif

// ��ʱxms

void Delay(unsigned int xms);
void Delay(unsigned int xms) //@11.0592MHz
{
    unsigned char i, j;
    while (xms) // while��������ʱ1ms�Ĵ���
    {
        _nop_();
        i = 2;
        j = 199;
        do
        {
            while (--j)
                ;
        } while (--i);
        --xms;
    }
}

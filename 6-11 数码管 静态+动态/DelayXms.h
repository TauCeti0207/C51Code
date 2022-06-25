// 延时xms
void Delay1ms(unsigned int xms) //@11.0592MHz
{
    unsigned char i, j;
    while (xms) // while里面是延时1ms的代码
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

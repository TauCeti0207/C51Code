#include <reg51.h>
#define uchar unsigned char


void main()
{
	uchar xdata* p1 = 0x07; // �ⲿRAM
	uchar data* p2 = 0x30; // �ڲ�RAM
	uchar data i;
	for(i = 0; i < 8; ++i)
	{
		*p2 = *p1;
		--p1, ++p2;
	}
	while(1); // ��̬ͣ��
}
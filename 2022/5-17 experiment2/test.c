#include <reg51.h>
#define uchar unsigned char


void main()
{
	uchar xdata* p1 = 0x07; // 外部RAM
	uchar data* p2 = 0x30; // 内部RAM
	uchar data i;
	for(i = 0; i < 8; ++i)
	{
		*p2 = *p1;
		--p1, ++p2;
	}
	while(1); // 动态停机
}
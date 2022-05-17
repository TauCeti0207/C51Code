#include <reg51.h>
#define uchar unsigned char

void main()
{
	// 内部RAM 初始化
	uchar data BUF1[10] = {0x30, 0x31, 0x32, 0x33, 0x34,
												0x35, 0x36, 0x37, 0x38, 0x39};
	uchar data BUF2[5], i;
	
	// 00 01 02 03 04 05 06 07 08 09			
	// 01 23 45 67 89											
	for(i = 0; i < 5; ++i)
	{
		BUF2[i] = (BUF1[2*i] & 0x0F) << 4;
		BUF2[i] = BUF2[i] + (BUF1[i*2 + 1] & 0x0F);
	}										
	
	while(1); // 动态停机
}
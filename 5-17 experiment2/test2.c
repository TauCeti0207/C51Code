#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int

void main()
{
	// �ڲ�RAM ��ʼ��
	uint data y = 0;
	uchar data *p = 0x20;
	uchar data byteLength; // ת���ֽ���
	uchar data i;
	uchar data tmp;
	byteLength = *p;
	for(i = 0; i < byteLength; ++i)
	{
		++p;
		// BCD��219H --> 001000011001(2) Ҳ����219(D) -->11011011(2) -->DB(H)
		// ת����Ӧʮ���Ƹ���λ*10+����λ���浽y���棬����2λ10���ƣ�*100
		// 02 19
		// 1.y=02
		// 2.y=2*100 + 1*10+9 = 219
		tmp = (*p >> 4)*10 + (*p & 0x0f);
		y = y*100 + tmp;
	}
	while(1); // ��̬ͣ��
}
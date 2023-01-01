#include <REGX52.H>

//void main()
//{
//	P2_4=1;	//C
//	P2_3=0;	//B
//	P2_2=0;	//A  CBA 为100 即Y4为0,其他为1
//	P0=0x7D;//为了显示6 0111 1101 让acdegef亮 b dp灭 
//	//abcdegf dp 对应为P00~P07
//	//即0x7D
//	while(1)
//	{
//		
//	}
//}

unsigned char NixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
void NixieTube(unsigned char Location,Number)//NixieTube 数码管
{
	switch(Location)		
	{
		case 1:P2_4=1;P2_3=1;P2_2=1;break;
		case 2:P2_4=1;P2_3=1;P2_2=0;break;
		case 3:P2_4=1;P2_3=0;P2_2=1;break;
		case 4:P2_4=1;P2_3=0;P2_2=0;break;
		case 5:P2_4=0;P2_3=1;P2_2=1;break;
		case 6:P2_4=0;P2_3=1;P2_2=0;break;
		case 7:P2_4=0;P2_3=0;P2_2=1;break;
		case 8:P2_4=0;P2_3=0;P2_2=0;break;
	}
	P0=NixieTable[Number];
}

void main()
{
	NixieTube(8,1);
	while(1)
	{
		
	}
}
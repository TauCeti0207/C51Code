
#ifndef __LCD__
#define __LCD__
sbit rs=P3^0;
sbit rw=P3^1;
sbit en=P3^2;
sbit key_left=P2^0;
sbit key_right=P2^1	;
#define DATAPORT P0
#define BUSY  0x80                               //常量定义
#define uchar unsigned char
#define uint unsigned int
uchar code opens[]="MACROSS";
uchar code opens1[]="FRONTIER";
uchar code hanzi[3][8]={0x1f,0x11,0x11,0x1f,0x11,0x11,0x1f,0x00,
					   0x0f,0x09,0x0f,0x09,0x0f,0x09,0x11,0x00,
					   0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02};
void open();
void ini(); 
void write_order(uchar order,uchar BusyC) ;
void write_date(uchar date);
void delay(uchar z);
void writeCG();
void ini()
{
	rw=0;
	en=0;
	P2=0xff;
	writeCG();
	write_order(0x38,1);
	write_order(0x0c,1);
	write_order(0x06,1);
	write_order(0x01,1);
	

}
void lcd_wait(void)
{
    DATAPORT=0xff;	     //读LCD前若单片机输出低电平,而读出LCD为高电平,则冲突,Proteus仿真会有显示逻辑黄色
	en=1;
    rs=0;   
    rw=1;   
    _nop_();
    _nop_();
	_nop_();
    while(DATAPORT&BUSY)  
	{  en=0;
	   _nop_();
	   _nop_();
	   en=1;
	   _nop_();
	   _nop_();
	   }	 
   en=0;	
	
}
void write_order(uchar com,uchar BusyC)
{
/*	rs=0;
	P2=order;
	delay(4);
	en=1;
	delay(4);
	en=0;	 */
	 if(BusyC)lcd_wait();
	DATAPORT=com;
    rs=0;                   // 选中指令寄存器
    rw=0;               	// 写模式	
    en=1;    
	_nop_();   
	_nop_();
	_nop_(); 
    en=0;
}
void write_date(uchar date)
{	
/*	rs=1;
	P2=date;
	delay(4);
	en=1;
	delay(4);
	en=0;	 */
	 lcd_wait( );             //检测忙信号    
	DATAPORT=date;
    rs=1;                // 选中数据寄存器
    rw=0;           	 // 写模式
    en=1;
    _nop_();
	_nop_();
	_nop_();
    en=0;
}
void delay(uchar z)
{
	uchar x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
void writeCG()
{
	uchar x,y;
	write_order(0x40,1);
	for(y=0;y<3;y++)
	{
		for(x=0;x<8;x++)
		{
			write_date(hanzi[y][x]);
		}
	}
	
}
void open()
{
	uchar num;
	write_order(0x80+0x10,1);
	for(num=0;opens[num]!='\0';num++)
	{
		write_date(opens[num]);
		//delay(20);
	}

	
			for(num=0;num<16;num++)
	{
		write_order(0x1c,1);
		//delay(200);
	}
			for(num=0;num<5;num++)
	{
		write_order(0x18,1);
		//delay(200);
	}
			for(num=0;num<4;num++)
	{
		write_order(0x1c,1);
		//delay(200);
	}
			for(num=0;num<3;num++)
	{
		write_order(0x18,1);
		//delay(200);
	}
			for(num=0;num<2;num++)
	{
		write_order(0x1c,1);
	//	delay(200);
	}
			for(num=0;num<1;num++)
	{
		write_order(0x18,1);
	//	delay(100);
	}
	write_order(0x80+0x53,1);
	for(num=0;opens1[num]!='\0';num++)
	{
		write_date(opens1[num]);
	//	delay(500);
	}
}
#endif

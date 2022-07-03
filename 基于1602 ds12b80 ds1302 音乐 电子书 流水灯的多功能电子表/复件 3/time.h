sbit rst=P1^4;
sbit sck=P1^5;
sbit datein=P1^6;
uchar month,day,hour,minite,second;
void readtime();
void write(uchar addr,uchar order);
uchar read(uchar addr);
void clockini();
void delay1();

void delay1()
{
	uchar x;
	for(x=10;x>0;x--);
}
void write(uchar addr,uchar order)
{
	uchar k;
	sck=0;
	delay1();
	rst=1;
	delay1();
	for(k=0;k<8;k++)
	{
		sck=0;
		datein=addr&&0x01;
		addr=addr>>1;
		delay1();
		sck=1;
		delay1();		
	}
	for(k=0;k<8;k++)
	{
		sck=0;
		datein=order&&0x01;
		order=order>>1;
		delay1();
		sck=1;
		delay1();
	}
	rst=0;
	delay1();
	sck=0;
}

uchar read(uchar addr)
{
	uchar k,receive;
	sck=0;
	delay1();
	rst=1;
	delay1();
	for(k=0;k<8;k++)
	{
		sck=0;
		datein=addr&&0x01;
		addr=addr>>1;
		delay1();
		sck=1;
		delay1();		
	}
	for(k=0;k<8;k++)
	{
		sck=0;
		receive=receive>>1;
		if(datein==1)
		{
			receive=receive|0x80;
		}
		delay1();		
		sck=1;
		delay1();		
	}
	rst=0;
	delay1();
	sck=0;
	return receive;	
}
void clockini()
{
	sck=0;
	rst=0;
	write(0x8e,0x00);
	write(0x82,0x00);
	write(0x84,0x16);
	write(0x86,0x19);
	write(0x88,0x09);
	write(0x90,0x0a6);
	write(0x80,0x00);
}

void readtime()
{
	month=read(0x89);
	day=read(0x87);
	hour=read(0x85);
	minite=read(0x83);
	second=read(0x81);
}


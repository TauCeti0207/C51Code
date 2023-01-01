#include <lxl_24C02C.h>


/*when crystal is 11.0592MHZ,one _nop_() takes 12*(1/11.0592MHZ)=1us.*/
void iic_delay_10us(uint multi)
{		
	uint num=10*multi;
	while(num--)_nop_();
}


void iic_start(void)
{
	sda=1;
	scl=1;
	iic_delay_10us(iic_delay);
	sda=0;
	iic_delay_10us(iic_delay);
	scl=0;
	iic_delay_10us(iic_delay);
	
}

void iic_stop(void)
{
	scl=0;
	sda=0;	
	scl=1;
	iic_delay_10us(iic_delay);
	sda=1;
	iic_delay_10us(iic_delay);
	scl=0;
	iic_delay_10us(iic_delay);
	
}

void iic_master_ack(void)
{
	sda=0;//master send a zero bit for ACK.
	scl=1;
	iic_delay_10us(iic_delay);
	scl=0;
	iic_delay_10us(iic_delay);
}

void iic_master_noack(void)
{
	sda=1;//master send a one bit for ACK.
	scl=1;
	iic_delay_10us(iic_delay);
	scl=0;
	iic_delay_10us(iic_delay);

}

uchar check_ack(void)
{
	uchar check;// if master receive a 1,something error happend;else receive a 0,slaver ack OK.
	sda=1;
	scl=1;
	iic_delay_10us(iic_delay);
	check=sda;
	iic_delay_10us(iic_delay);
	scl=0;
	iic_delay_10us(iic_delay);
	if(check)return 1;
	return 0;
	
}
void Master_writeData(uchar masterWriteDate)
{

	uchar i;
	for(i=0;i<8;i++)
	{
		if(masterWriteDate&0x80)
			iic_master_noack();		//write a 1.
		else
			iic_master_ack();			//write a 0.
		masterWriteDate<<=1;
	}
}

uchar Master_readData(void)
{

	uchar i,recvData=0;

	for(i=0;i<8;i++)
	{
		sda=1;
		scl=1;		
		iic_delay_10us(iic_delay);
		recvData=_cror_(sda,i+1)|recvData;
		iic_delay_10us(iic_delay);
		scl=0;
	}
	return recvData;
}
/*single chip 24C02C A0-2 are all zero,so write command is 0xA0,read command is 0xA1,
modify the value in file : lxl_24C02C.h */
void Master_writeByte(uchar addr,uchar Byte)
{
	iic_start();
	Master_writeData(WRITE);
	while(check_ack());
	Master_writeData(addr);
	while(check_ack());
	Master_writeData(Byte);
	while(check_ack());	
	iic_stop();
}

uchar Master_readByte(uchar addr)
{
	uchar readByte;
	iic_start();
	Master_writeData(WRITE);
	while(check_ack());
	Master_writeData(addr);
	while(check_ack());
	iic_start();
	Master_writeData(READ);
	while(check_ack());
	readByte=Master_readData();
	iic_master_noack();
	iic_stop();
	return readByte;
}
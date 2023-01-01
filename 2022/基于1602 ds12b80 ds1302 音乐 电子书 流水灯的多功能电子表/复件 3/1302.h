sbit ds1302_RST =P2^4;
sbit ds1302_IO  =P2^6;
sbit ds1302_SCLK=P2^5;
sbit ACC0=ACC^0;
sbit ACC7=ACC^7;

uchar  initial_time[]={0x40,0x07,0x12};
uchar  now_time[5],ring_time[3];
void init_ds1302();
void get_ds1302() ;
void set_ds1302(uchar taddr,uchar *P1302);
uchar read_data_ds1302(uchar taddr);
void write_data_ds1302(uchar taddr,uchar tdata);
void  write_Byte(uchar tdata);
uchar  read_Byte();
void read_ring();



uchar  read_Byte()
{
   uchar i;
   for(i=8;i>0;i--)
   {
     ACC=ACC>>1;
     ACC7=ds1302_IO;
     ds1302_SCLK=1;
     ds1302_SCLK=0;					
   } 
   return(ACC);
}



void  write_Byte(uchar tdata)
{
   uchar i;
   ACC=tdata;
   for(i=8;i>0;i--)
   {
     ds1302_IO=ACC0;
     ds1302_SCLK=1;
     ds1302_SCLK=0;
     ACC=ACC>>1;
   }

}


void write_data_ds1302(uchar taddr,uchar tdata)
{
   ds1302_RST=0;
   ds1302_SCLK=0;
   ds1302_RST=1;
   write_Byte(taddr);
   write_Byte(tdata); 
   ds1302_RST=0;
   ds1302_SCLK=1;
}


uchar read_data_ds1302(uchar taddr)
{  
   uchar tdata;
    
   ds1302_RST=0;
   ds1302_SCLK=0;
   ds1302_RST=1;
   write_Byte(taddr);
   tdata=read_Byte(); 
   ds1302_RST=0;
   ds1302_SCLK=1;
   return(tdata);
}



void set_ds1302(uchar taddr,uchar *P1302)
{
    uchar i;
    write_data_ds1302(0x8e,0x00);          
    for(i =3; i>0; i--)
    { 
       write_data_ds1302(taddr,*P1302);    
        P1302++;
        taddr+=2;
    }
   write_data_ds1302(0x8e,0x80);          
}


void get_ds1302() 
{
    uchar k;
    uchar taddr = 0x81;
    for (k=0; k<5; k++)
    {
        now_time[k] = read_data_ds1302(taddr);
        taddr+=2;

    }
}

void read_ring()
{
	uchar k;
	uchar taddr=0xc1;
	for(k=0;k<3;k++)
	{
		ring_time[k]=read_data_ds1302(taddr);
		taddr+=2;
	}
}
void init_ds1302()
{
    ds1302_RST=0;
    ds1302_SCLK=0;
}



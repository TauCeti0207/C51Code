#include<REG52.H>
#include<INTRINS.H>

#define TIME (0X10000-50000)
#define FLAG 0XEF		//���ӱ�־
//��������ͼ
sbit rst=P3^5;
sbit clk=P3^4;
sbit dat=P3^3;
sbit rs=P1^5;
sbit rw=P1^6;
sbit e=P1^7;
sbit DQ=P1^4;        //�¶������

sbit ACC_7=ACC^7;
//ȫ�ֱ�������������
unsigned char i=20,j,time1[16];	
unsigned char alarm[2],time2[15],time[3];
unsigned char code Day[]={31,28,31,30,31,30,31,31,30,31,30,31};//12���µ��������(������)
//���ɱ�
unsigned int code table1[]={64260,64400,64524,64580,64684,64777,
64820,64898,64968,65030,65058,65110,65157,65178,65217};
//�������ֵ���ʱʱ��
unsigned char code table2[]={0x82,1,0x81,0xf4,0xd4,0xb4,0xa4,
0x94,0xe2,1,0xe1,0xd4,0xb4,0xc4,0xb4,4,0};
//LCD�Խ���
unsigned char code tab[]={0x18,0x1b,5,4,4,5,3,0,
0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,//��
0x0f,0x09,0x0f,0x09,0x0f,0x09,0x11,0x00,//��
0x0f,0x09,0x09,0x0f,0x09,0x09,0x0f,0x00};//�� 
//*******�¶�С�������ò��**********//
unsigned char code ditab[16]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};
//�������õ�ȫ�ֱ���
unsigned char th1,tl1;
unsigned char temp_data[2]={0x00,0x00};               // �����¶��ݷ�
bit flag;		//18b20���ڱ�־λ
/***********11΢����ʱ����**********/
delay(unsigned int t)
{
	for(;t>0;t--);
}
/***********18B20��λ����**********/
RST18B20(void)
{
	unsigned char i;
	DQ=1;_nop_();_nop_();
	DQ=0;    //
	delay(50); // 550us
	DQ=1;    // 
	delay(6);  // 66us
	for(i=0;i<0x30;i++)
	{
		if(!DQ)
			goto d1;			
	}
	flag=0;		//���־λ,��ʾds1820������
	DQ=1;
	return;
d1:	delay(45);    //��ʱ500us
	flag=1;
	DQ=1;		//�ñ�־λ,��ʾds1820����
} 
/**********18B20д�����*********/
//�� 1-WIRE ������дһ���ֽ�
void Write1Byte18B20(unsigned char val)
{
	unsigned char i;
	for (i=8; i>0; i--) //
	{
		DQ=1;_nop_();_nop_(); 
		DQ=0;_nop_();_nop_();_nop_();_nop_();_nop_();//5us
		DQ=val&0x01;      //���λ�Ƴ�
		delay(6);           //66us
		val=val/2;          //����һλ
	}
	DQ=1;
	delay(1);  
}
/*********18B20��1���ֽں���********/
//�������϶�ȡһ���ֽ�
unsigned char Read1Byte18B20(void)
{
	unsigned char i;
	unsigned char value=0;
	for (i=8;i>0;i--)
	{
		DQ=1;_nop_();_nop_();
		value>>=1;
		DQ=0;             //
		_nop_();_nop_();_nop_();_nop_();   //4us
		DQ=1;_nop_();_nop_();_nop_();_nop_();   //4us 
		if(DQ)
			value|=0x80;
		delay(6);           //66us
	}
	DQ=1;
	return(value);
}
/***********�����¶Ⱥ���**********/
Read18B20()
{
	RST18B20();       			//���߸�λ
	if(!flag)		//�ж�ds1820�Ƿ����?��ds18b20�������򷵻�
		return;				
		
	//Write1Byte18B20(0xCC); 			// Skip ROM
	//Write1Byte18B20(0x44); 			// ��ת������
	//delay(70);
	
	Write1Byte18B20(0xCC); 			//��Skip ROM����
	Write1Byte18B20(0xBE); 			//��������
	temp_data[0]=Read1Byte18B20();   //�¶ȵ�8λ
	temp_data[1]=Read1Byte18B20();   //�¶ȸ�8λ	
	RST18B20();
	Write1Byte18B20(0xCC); 			// Skip ROM
	Write1Byte18B20(0x44); 			// ��ת������
}
/***********�¶����ݴ�����**********/
DealTempData()
{
	unsigned char n=0,m;       
	if(temp_data[1]>127)//���¶�����
	{
		temp_data[1]=(256-temp_data[1]);
		temp_data[0]=(256-temp_data[0]);
		n=1;
	}
	time2[13]=ditab[temp_data[0]&0x0f]+'0';
	time2[12]='.';
	m=((temp_data[0]&0xf0)>>4)|((temp_data[1]&0x0f)<<4);//
	if(n)
	{
		m-=16;
	}
	time2[9]=m/100+'0';
	time2[11]=m%100;
	time2[10]=time2[11]/10+'0';
	time2[11]=time2[11]%10+'0';
	if(time2[9]=='0')//���λΪ0ʱ������ʾ
	{
		time2[9]=0x20;	
		if(n)//���¶�ʱ���λ��ʾ"-"
		{
			time2[9]='-';
		}
		if(time2[10]=='0')
		{
			if(n)
			{
				time2[10]='-';
				time2[9]=0x20;
			}
			else
				time2[10]=0x20;
			if(time2[11]=='0'&&time2[13]=='0')
				time2[11]=time2[12]=0x20;
		}			
	}
}
delay1ms(unsigned char time)		//��ʱ1ms
{
	unsigned char i,j;
	for(i=0;i<time;i++)
	{
		for(j=0;j<250;j++);
	}
}

//ʹ��1602
EnableLCD()
{
	rs=0;
	rw=0;
	e=0;
	delay1ms(3);
	e=1;
}
//дLCD����
WriteLCD(unsigned char i)
{
	P0=i;
	rs=1;
	rw=0;
	e=0;
	delay1ms(2);
	e=1;
}
//дLCD�ڲ���ַ����
WriteLCDRAM(unsigned char data *address,m)
{
	unsigned char i,j;
	for(i=0;i<m;i++,address++)
	{
		j=*address;
		WriteLCD(j);
	}
}
//LCD��ʾ
LCDSHOW()
{
	P0=0XC;				//��ʾ����������
	EnableLCD();
	P0=0x80;			//д����ʾ��ʼ��ַ
	EnableLCD();
	WriteLCDRAM(time1,16);
	P0=0xc1;			//д����ʾ��ʼ��ַ
	EnableLCD();
	WriteLCDRAM(time2,15);
}
//�Խ��ֺ�����
DesignHZ()
{
	unsigned char i;
	P0=0x40;
	EnableLCD();
	for(i=0;i<32;i++)
	{
		WriteLCD(tab[i]);
		delay1ms(2);	
	}
}

//дDS1302�ӳ���
WriteDS1302(unsigned char address)
{
	unsigned char i;
	clk=0;
	_nop_();
	rst=1;
	_nop_();
	for(i=0;i<8;i++)
	{
		dat=address&1;
		_nop_();
		clk=1;
		address>>=1;
		clk=0;
	}
}
//��DS1302�ӳ���
unsigned char ReadDS1302()
{
	unsigned char i,j=0;
	for(i=0;i<8;i++)
	{
		j>>=1;
		_nop_();
		clk=0;
		_nop_();
		if(dat)
			j|=0x80;
		_nop_();
		clk=1;
	}
	return(j);
}
//������ʾ���ݳ�ʼ��
TimeInit()
{
	time1[1]=time1[13]=time2[8]=time2[9]=time2[10]=0x20,time2[14]=0;
	time1[6]=1,time1[9]=2,time1[12]=3,time1[2]='2',time1[3]='0';
	time1[14]='W',time2[2]=time2[5]=':';
	WriteDS1302(0xc1);
	alarm[0]=ReadDS1302();
	rst=0;
	WriteDS1302(0xc3);
	alarm[1]=ReadDS1302();
	rst=0;
	WriteDS1302(0xc5);
	time1[0]=ReadDS1302();
	rst=0;
}
//��ȡʱ��
ReadTime()
{
	unsigned char i,m,n;
	WriteDS1302(0x8d);			//��ȡ���
	m=ReadDS1302();
	rst=0;
	time1[4]=m/16+0x30;
	time1[5]=m%16+0x30;
	WriteDS1302(0x8b);			//��ȡ����
	m=ReadDS1302();
	rst=0;
	time1[15]=m+0x30;
	for(i=7,n=0x89;i<11;i+=3,n-=2)	//��ȡ�·ݺ�����
	{
		WriteDS1302(n);
		m=ReadDS1302();
		rst=0;
		time1[i]=m/16+0x30;
		time1[i+1]=m%16+0x30;
	}
	for(m=0,i=0,n=0x85;i<7;i+=3,n-=2,m++)	//��ȡʱ,��,��
	{
		WriteDS1302(n);
		time[m]=ReadDS1302();
		rst=0;
		time2[i]=time[m]/16+0x30;
		time2[i+1]=time[m]%16+0x30;
	}
}
time0() interrupt 1 using 1
{
	i--;
	if(i==0)
	{
		if(j!=0)
			j--;		 
		i=20;
	}
	TH0=TIME/256,TL0=TIME%256;		
}
//���Ӳ���
intime1() interrupt 3
{
	TH1=th1,TL1=tl1;
	P3_2=!P3_2;
}
//��ʾ������������
showalarm()
{
	unsigned char i,j,a,b,n;
	ET1=1;
	for(j=0;j<6;j++)
	{
		i=0;
		while(1)
		{
			a=table2[i];
			if(a==0)
				break;
			b=a&0xf;
			a>>=4; 
			if(a==0)
			{
				TR1=0;
				goto  D1;
			}
			a=((--a)<<1)/2;
			TH1=th1=table1[a]/256,TL1=tl1=table1[a]%256;
			TR1=1;
D1:			do
			{
				b--;
				for(n=0;n<3;n++)
				{
					ReadTime();
					LCDSHOW();
					P2=0xf7;
					if(P2==0xe7)
					{
						delay1ms(100);
						if(P2==0xe7)
						{
							TR1=0;
							ET1=0;
							return;
						}	
					}
				}
			}while(b!=0);
			i++;				
		}
		TR1=0;	   	
	}
	ET1=0;
}
//�������ڵı䶯�Զ���������
unsigned char setweek()
{
	unsigned char i=5,j,n;
	j=(time1[4]&0xf)*10+(time1[5]&0xf);
	n=j/4;
	i=i+5*n;
	n=j%4;
	if(n==1)
		i+=2;
	else if(n==2)
		i+=3;
	else if(n==3)
		i+=4;
	j=(time1[7]&0xf)*10+(time1[8]&0xf);
	if(j==2)
		i+=3;
	else if(j==3)
		i+=3;	
	else if(j==4)
		i+=6;	
	else if(j==5)
		i+=1;	
	else if(j==6)
		i+=4;	
	else if(j==7)
		i+=6;	
	else if(j==8)
		i+=2;	
	else if(j==9)
		i+=5;
	else if(j==11)
		i+=3;	
	else if(j==12)
		i+=5;
	if(n==0)
		if(j>2)	
			i++;	
	j=(time1[10]&0xf)*10+(time1[11]&0xf);
	i+=j;
	i%=7;
	if(i==0)
		i=7;
	return(i);	
}
//����ʱ��
settime()
{
	unsigned char i=0x85,year,month,day,n;
	time2[6]=time2[7]=0x30,time1[14]=time1[15]=0x20;
	LCDSHOW();
	while(1)
	{		
		P0=0xe;			//��ʾ��������꿪
		EnableLCD();
		P0=i;			//�����
		EnableLCD();
		P2=0xf7;
		if(P2!=0XF7)
		{
			delay1ms(100);				//��ʱ0.1sȥ����
			if(P2!=0XF7)
			{
				j=7;
				if(P2==0X77)
				{
					i+=3;
					if(i==0x8e)
						i=0xc2;
					else if(i>0xc5)
						i=0x85;
				}
				else if(P2==0xb7)
				{
					year=(time1[4]&0xf)*10+(time1[5]&0xf);
					month=(time1[7]&0xf)*10+(time1[8]&0xf);
					day=(time1[10]&0xf)*10+(time1[11]&0xf);
					if(i==0x85)
					{
						year++;
						if(year>99)
							year=0;
						if((year%4)!=0)
							if(month==2&&day==29)
								day=28;
					}
					else if(i==0x88)
					{
						month++;
						if(month>12)
							month=1;
						if(day>Day[month-1])
						{
							day=Day[month-1];
							if(month==2&&(year%4)==0)	
								day=29;
						}							
					}
					else if(i==0x8b)
					{
						day++;
						if(day>Day[month-1])
						{
							if(month==2&&(year%4)==0)
							{
								if(day>29)
									day=1;
							}
							if(month!=2)
								day=1;
						}
					}
					else if(i==0xc2)
					{
						n=(time2[0]&0xf)*10+(time2[1]&0xf);
						n++;
						if(n>23)
							n=0;
						time2[0]=n/10+0x30;
						time2[1]=n%10+0x30;
					}
					else
					{
						n=(time2[3]&0xf)*10+(time2[4]&0xf);
						n++;
						if(n>59)
							n=0;
						time2[3]=n/10+0x30;
						time2[4]=n%10+0x30;
					}
					time1[4]=year/10+0x30;
					time1[5]=year%10+0x30;
					time1[7]=month/10+0x30;
					time1[8]=month%10+0x30;
					time1[10]=day/10+0x30;
					time1[11]=day%10+0x30;
					LCDSHOW();
				}
				else if(P2==0xd7)
				{
					WriteDS1302(0x8c);
					WriteDS1302((time1[4]&0xf)*16+(time1[5]&0xf));
					rst=0;
					WriteDS1302(0x8a);
					WriteDS1302(setweek());
					rst=0;
					for(i=7,n=0x88;i<11;i+=3,n-=2)
					{	
						WriteDS1302(n);
						WriteDS1302((time1[i]&0xf)*16+(time1[i+1]&0xf));
						rst=0;
					}
					for(i=0;i<7;i+=3,n-=2)
					{
						WriteDS1302(n);
						WriteDS1302((time2[i]&0xf)*16+(time2[i+1]&0xf));
						rst=0;
					}
					TR0=0;
					time1[14]='W';
					return;
				}
				else
				{
					TR0=0;
					time1[14]='W';
					return;
				}
			}
		}	
		if(j==0)
		{	
			TR0=0;
			time1[14]='W';
			return;
		}
	}
}
//��������
setalarm()
{
	unsigned char i,n;
	for(i=1;i<16;i++)
	{
		time1[i]=0x20;
	}	
	time2[0]=alarm[0]/16+0x30;
	time2[1]=(alarm[0]&0xf)+0x30;
	time2[3]=alarm[1]/16+0x30;
	time2[4]=(alarm[1]&0xf)+0x30;
	time2[6]=time2[7]=0x30;
	LCDSHOW();
	i=0xc2;
	while(1)
	{
		P0=0xe;			//��ʾ��������꿪
		EnableLCD();
		P0=i;			//�����
		EnableLCD();
		P2=0xf7;
		if(P2!=0XF7)
		{
			delay1ms(100);				//��ʱ0.1sȥ����
			if(P2!=0XF7)
			{
				j=7;
				if(P2==0X77)
				{
					i+=3;
					if(i>0xc5)
						i=0xc2;
				}
				else if(P2==0xb7)
				{
					if(i==0xc2)
					{
						n=(time2[0]&0xf)*10+(time2[1]&0xf);
						n++;
						if(n>23)
							n=0;
						time2[0]=n/10+0x30;
						time2[1]=n%10+0x30;
					}
					else
					{
						n=(time2[3]&0xf)*10+(time2[4]&0xf);
						n++;
						if(n>59)
							n=0;
						time2[3]=n/10+0x30;
						time2[4]=n%10+0x30;
					}
					LCDSHOW();
				}
				else if(P2==0xd7)
				{
					WriteDS1302(0xc0);
					WriteDS1302((time2[0]&0xf)*16+(time2[1]&0xf));
					rst=0;
					WriteDS1302(0xc2);
					WriteDS1302((time2[3]&0xf)*16+(time2[4]&0xf));
					rst=0;
					time1[0]=FLAG;
					WriteDS1302(0xc4);
					WriteDS1302(time1[0]);
					rst=0;
					TR0=0;
					TimeInit();
					return;
				}
				else
				{
					TR0=0;
					TimeInit();
					return;
				}
			}
		}	
		if(j==0)
		{	
			TR0=0;
			TimeInit();
			return;
		}
	}
}
main()
{
	IE=0X82;
	TMOD=0x11;
	WriteDS1302(0x8E);			//��ֹд����
	WriteDS1302(0);
	rst=0;
	P0=1;				//��������긴λ
	EnableLCD();
	P0=0X38;			//������ʾģʽ:8λ2��5x7����
	EnableLCD();
	P0=6;				//���ֲ���,����Զ�����
	EnableLCD();
	DesignHZ();			//�Խ���
	TimeInit();
	while(1)
	{	
		ReadTime();				//��ȡʱ��
		Read18B20();        	//����18B20�¶�����
		DealTempData();        	//�����¶�����
		LCDSHOW();				//��ʾʱ��
		if(time1[0]!=0x20)
			if(time[0]==alarm[0])
				if(time[1]==alarm[1])
					if(time[2]==0)
						showalarm();
		P2=0xf7;
		if((P2&0XF0)!=0XF0)
		{
			delay1ms(100);				//��ʱ0.1sȥ����
			if((P2&0XF0)!=0XF0)
			{
				j=7;
				TH0=TIME/256,TL0=TIME%256;
				TR0=1;
				if(P2==0x77)
				{
					settime();
				}
				else if(P2==0XB7)
				{
					setalarm();
				}
				else if(P2==0XD7)
				{
					TR0=0;
					if(time1[0]==FLAG)
						time1[0]=0x20;
					else
						time1[0]=FLAG;
					WriteDS1302(0xc4);
					WriteDS1302(time1[0]);
					rst=0;
				}
			}
		}
		delay1ms(100);	
	}
}
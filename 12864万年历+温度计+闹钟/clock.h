#ifndef _REAL_TIMER_DS1302
#define _REAL_TIMER_DS1302
/*****************************Ԥ����**************************************/
#define uchar unsigned char
#define uint unsigned int/***************************DS1302�ܽ�����****************************/
sbit clock_rst = P2^4;
sbit clock_io  = P2^3;       
sbit clock_sclk= P2^2;
/*********************Ϊ�˱�̷��㶨���λ����**********************/
sbit ACC0 = ACC ^ 0;
sbit ACC7 = ACC ^ 7;
#define second_address	0x80
#define minute_address	0x82
#define hour_address	0x84 
#define day_address	    0x86
#define month_address	0x88
#define year_address	0x8C
/********************************************************************************************
* �������ƣ�Clock_Write_Byte()
* ��    �ܣ���ʱ��DS1302д��һ���ֽ�
* ��ڲ�����temp
* ���ڲ�������
*********************************************************************************************/
void Clock_Write_Byte(uchar temp)
{
    uchar i;
    ACC=temp;
    for(i=8; i>0; i--)
    {
        clock_io = ACC0;           	//�൱�ڻ���е� RRC
        clock_sclk = 1;
        clock_sclk = 0;
        ACC = ACC >> 1; 
    } 
}
/********************************************************************************************
* �������ƣ�Clock_Read_Byte()
* ��    �ܣ���ʱ��DS1302��ȡһ���ֽ�
* ��ڲ�������
* ���ڲ�����ACC
*********************************************************************************************/
uchar Clock_Read_Byte(void)
{
	uchar i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         			//�൱�ڻ���е� RRC 
        ACC7= clock_io;
		clock_sclk = 1;
        clock_sclk = 0;
	} 
    return(ACC);
}
/********************************************************************************************
* �������ƣ�Clock_Write_Time()
* ��    �ܣ���ʱ��DS1302д��һ��ʱ��
* ��ڲ�����ʱ���ַ  
* ���ڲ�������
*********************************************************************************************/
void Clock_Write_Time(uchar address, uchar temp)
{
	clock_sclk=0;
	clock_rst=0;
	clock_rst=1;
	Clock_Write_Byte(address);
	Clock_Write_Byte(temp);
	clock_rst=0;
	clock_sclk=1;
}
/********************************************************************************************
* �������ƣ�Clock_Read_Time()
* ��    �ܣ���ʱ��DS1302����һ��ʱ��
* ��ڲ�����ʱ���ַ  
* ���ڲ�����ʱ������
*********************************************************************************************/
uchar Clock_Read_Time(uchar address)
{
	uchar temp=0;
	clock_sclk=0;
	clock_rst=0;
	clock_rst=1;
	Clock_Write_Byte( address|0x01 );
	temp=Clock_Read_Byte();
	clock_rst=0;
	clock_sclk=1;
	return(temp);
}

/********************************************************************************************
* �������ƣ�Clock_Initial()
* ��    �ܣ�ʱ�ӳ�ʼ��
* ��ڲ�����ʱ���ַ   ʱ������
* ���ڲ�����
*********************************************************************************************/
void Clock_Initial(uchar *clock_time )
{
    Clock_Write_Time(0x8e,0x00);                //WP=0 д����
    *clock_time &= 0x7f;                             //���λΪ 0 ʱ��оƬ����
	Clock_Write_Time( second_address, * clock_time );   //��
    clock_time ++;
    Clock_Write_Time( minute_address, * clock_time );    //��
    clock_time ++;
    Clock_Write_Time( hour_address, * clock_time );     //ʱ
    clock_time ++;
    Clock_Write_Time( day_address, * clock_time );      //��
    clock_time ++;
    Clock_Write_Time( month_address, * clock_time);   //��
	clock_time ++;
    Clock_Write_Time( year_address, * clock_time );    //��
    Clock_Write_Time( 0x8e,0x80);              //WP=1 д����	
}

/********************************************************************************************
* �������ƣ�Clock_Fresh( uchar * clock_time )
* ��    �ܣ���ʱ��DS1302����ʱ��
* ��ڲ�����ʱ���ַ  
* ���ڲ�����ʱ������
*********************************************************************************************/
void Clock_Fresh(uchar * clock_time )
{
	* clock_time = Clock_Read_Time( second_address );  //����
	clock_time ++;
	* clock_time = Clock_Read_Time( minute_address ); //����
	clock_time ++;
	* clock_time = Clock_Read_Time( hour_address ); //��ʱ
    clock_time ++;
    * clock_time = Clock_Read_Time( day_address ); //����	
	clock_time ++;
    * clock_time = Clock_Read_Time( month_address ); //����
	clock_time ++;
	*clock_time  = Clock_Read_Time( year_address ); //����	
}
/********************************************************************************/
#endif

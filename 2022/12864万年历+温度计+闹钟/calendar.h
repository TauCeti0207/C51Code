#ifndef _SUN_MOON
#define _SUN_MOON
/*************************************************************************/
#define uchar unsigned char
#define uint unsigned int

/********************************************************************************
* ����: get_moon_day(uchar month_p,uint table_addr)
* ����: ��ȡ���ݱ���ũ���Ĵ��»�С�� ��������·���1, С�·���0
* ��ڲ���:
* ���ڲ���:
*********************************************************************************/
bit get_moon_day( uchar month_p,uint calendar_address )
{
    uchar temp,temp1;

	temp1=(month_p+3)/8;
	temp=0x80>>((month_p+3)%8);
	temp=year_code[calendar_address+temp1]&temp;
	if(temp==0){return(0);}else{return(1);}
 
 /*   switch(month_p)
	{
        case 1: { temp = year_code[calendar_address] & 0x08; if(temp==0) return(0); else return(1); }
        case 2: { temp = year_code[calendar_address] & 0x04; if(temp==0) return(0); else return(1); }
        case 3: { temp = year_code[calendar_address] & 0x02; if(temp==0) return(0); else return(1); }
        case 4: { temp = year_code[calendar_address] & 0x01; if(temp==0) return(0); else return(1); }
        case 5: { temp = year_code[calendar_address + 1] & 0x80; if(temp==0) return(0); else return(1); }
        case 6: { temp = year_code[calendar_address + 1] & 0x40; if(temp==0) return(0); else return(1); }
        case 7: { temp = year_code[calendar_address + 1] & 0x20; if(temp==0) return(0); else return(1); }
        case 8: { temp = year_code[calendar_address + 1] & 0x10; if(temp==0) return(0); else return(1); }
        case 9: { temp = year_code[calendar_address + 1] & 0x08; if(temp==0) return(0); else return(1); }
        case 10: { temp = year_code[calendar_address + 1] & 0x04; if(temp==0) return(0); else return(1); }
        case 11: { temp = year_code[calendar_address + 1] & 0x02; if(temp==0) return(0); else return(1); }
        case 12: { temp = year_code[calendar_address + 1] & 0x01; if(temp==0) return(0); else return(1); }
        case 13: { temp = year_code[calendar_address + 2] & 0x80; if(temp==0) return(0); else return(1); } 
    }
	���ϵĳ���������ĳ���ȼۣ�����ĳ��������һЩ������
	*/



}
/**************************************************************************
* ����: void Calendar_Convert( uchar * clock_time )
* ����: ����BCD���������ݣ� ���BCD��������( 1901 - 2099 )
* ��ڲ���: c_flag:���������ͱ�־  clock_time: ʱ�ӵ�ַ
* ���ڲ���: ��
* ˵��: c_flag = 0 :21���� c_flag = 1 :19���� 
*****************************************************************************/
void Calendar_Convert( uchar c_flag, uchar * clock_time )
{
   bit flag_month, flag_year;
   uchar year, month, day, month_point; //���� �� �� ��
   uchar temp1, temp2, temp3;
   uint calendar_address; //����ũ����ַ
   uint day_number;
   uchar clock_moon[3]; //��������


   clock_time += 3; //ָ����
   day = ( * clock_time >> 4 ) * 10 + ( *clock_time & 0x0f ); //BCDת��ʮ����
   clock_time ++; //ָ����
   month = ( * clock_time >> 4 ) * 10 + ( * clock_time & 0x0f ); //BCDת��ʮ����
   clock_time ++; //ָ����
   year = ( * clock_time >> 4 ) * 10 + ( * clock_time & 0x0f ); //BCDת��ʮ���� 
  
   //��λ������ַ
   if( c_flag == 0 )
      calendar_address = ( year + 99 ) * 3;  
   else
      calendar_address = ( year - 1 ) * 3;

   //����(���³�һ)���ڵ������·�
   temp1 = year_code[ calendar_address + 2 ] & 0x60; //Bit6~~Bit5:�������ڵ������·�
   temp1 >>= 5 ;
   //����(���³�һ)���ڵ���������
   temp2 = year_code[ calendar_address + 2 ] & 0x1f; //Bit4~~Bit0:�������ڵ���������
  
   //���㴺��(���³�һ)�뵱��Ԫ��{ 1��1��(����) }������������ֻ����������1�� �� 2��
   /*if( temp1 == 1 )
      temp3 = temp2 - 1;
   else
      temp3 = temp2 + 31 - 1;
   	 */

	temp3=temp2-1;
	if(temp1!=1) temp3+=0x1f;
   

   //�����������뵱��Ԫ��{ 1��1��(����) }������
   if( month < 10 )
      {day_number = day_code1[ month - 1 ] + day  ;} 
   else
      {day_number = day_code2[ month - 10 ] + day ;}

   //����������´���2 �Ҹ����2��Ϊ���£�������1 
   //����ָ�ľ������������ջ����������µ�һ�ꣻ
   //��������һ���ڶ��¼�һ�죬��һ��������գ�
   //ũ������һ����������ʮ��������ÿ���������ӵ�һ���½������¡�
   if( ( month <= 2 ) || ( year % 0x04!= 0) ) day_number-=1;
   //   day_number ++;

  //   if ((month<2)||(year%0x04!=0))
  //   day_number-=1;

   //�ж������� �ڴ���(���³�һ) ֮ǰ ���� ֮��
   if( day_number >= temp3 ) //�����ڴ���֮�� ���� ���ڵ���
   {
      day_number -= temp3;
      month = 1;
      month_point = 1; // month_point Ϊ�·�ָ���������ڴ���ǰ���Ǵ���
	  flag_month = get_moon_day( month_point, calendar_address ); //���������µĴ�С  ���·���1  С�·���0
	  flag_year = 0;
	 /* if( flag_month )
	     temp1 = 30; //����30��
	  else
 	     temp1 = 29; //С��29��
      */
      if (flag_month==0) {temp1=29;} else{temp1=30;}

	  //�������ڵ��·�
	  temp2 = year_code[ calendar_address ] & 0xf0;
	  temp2 >>= 4; //��ȡ����λ  ������0 ��ʾû������

	  while( day_number >= temp1 )
	  {
	     day_number -= temp1;
         month_point ++;
         if( month == temp2 )
         {
            flag_year = ~ flag_year; 
            if( flag_year == 0 ) 
               month +=1;
         }
         else
            month ++ ;
         flag_month = get_moon_day( month_point, calendar_address );
         if( flag_month )
            temp1 = 30;
         else
            temp1 = 29;
       }
       day = day_number + 1;
   }
   else //�����ڴ���֮ǰʹ�����´����������
   {
      temp3 -= day_number;
      if( year == 0 )
         { year = 0xe3; c_flag = 1; }
      else
         year -= 1;
      calendar_address -= 3;
      month = 0xc;
      temp2 = year_code[ calendar_address ] & 0xf0;
      temp2 >>= 4; //��ȡ��4λ
      flag_year=0;
	  if( temp2 == 0 )
         month_point = 12;
      else
         month_point = 13;
      //flag_year = 0;
      flag_month = get_moon_day( month_point, calendar_address );
      if( flag_month )
         temp1 = 30;
      else
         temp1 = 29;
      while( temp3 > temp1 )
      {
         temp3 -= temp1;
         month_point --;
         if( flag_year == 0 )
         month -=1;
         if( month == temp2 )
         flag_year = ~ flag_year;
         flag_month = get_moon_day( month_point, calendar_address );
         if( flag_month )
            temp1 = 0x1e;
         else
            temp1 = 0x1d;
         
      }
      day = temp1 - temp3 + 1;
   }
   //HEX->BCD ,���������,������ת��ΪBCD����
   temp1 = year / 10;
   temp1 <<= 4;
   clock_moon[2] = temp1 | ( year % 10 );
 
   temp1 = month / 10;
   temp1 <<= 4;
   clock_moon[1] = temp1 | ( month % 10 );
 
   temp1 = day / 10;
   temp1 <<= 4;
   clock_moon[0] = temp1 | ( day % 10 );
   Lcd_Lunar_Calendar( clock_moon );
}

/********************************************************************************
* ����: void Week_convert( uchar * clock_time )
* ����: ����BCD�������ݣ����BCD����
* ��ڲ���: 
* ���ڲ���:
* ˵��: �㷨: �� ���� + ��� + �������� + ��У�� ) / 7 �������������� ����������ֲ��� 3 �·�����֮�� Ҫ��һ�� ��
********************************************************************************/
void Week_Convert( bit c, uchar * clock_time )
{
   uchar year, month, day; //���� �� �� ��
   uchar temp;

   clock_time += 3; //ָ����
   day = ( * clock_time >> 4 ) * 10 + ( *clock_time & 0x0f ); //BCDת��ʮ����
   clock_time ++; //ָ����
   month = ( * clock_time >> 4 ) * 10 + ( * clock_time & 0x0f );
   clock_time ++; //ָ����
   year = ( * clock_time >> 4 ) * 10 + ( * clock_time & 0x0f ); 

   if( c == 0 ) //���Ϊ21����,�������100
      year += 100;
   temp = year / 4; //����������ֻ��1900��֮���
   temp = year + temp;
   temp = temp % 0x07; //Ϊ��ʡ��Դ,�Ƚ���һ��ȡ��,����������0xff,����ʹ����������
   temp = temp + day + table_week[ month - 1 ];
   if( ( year % 4  == 0 ) && ( month <3  ) )
      temp -=1;
   Lcd_Week( temp % 7 );
}
/********************************************************************************/
#endif
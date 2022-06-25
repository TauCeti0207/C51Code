#ifndef _SUN_MOON
#define _SUN_MOON
/*************************************************************************/
#define uchar unsigned char
#define uint unsigned int

/********************************************************************************
* 名称: get_moon_day(uchar month_p,uint table_addr)
* 功能: 读取数据表中农历的大月或小月 ，如果大月返回1, 小月返回0
* 入口参数:
* 出口参数:
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
	以上的程序与上面的程序等价，上面的程序更精简一些！！！
	*/



}
/**************************************************************************
* 名称: void Calendar_Convert( uchar * clock_time )
* 功能: 输入BCD的阳历数据， 输出BCD阴历数据( 1901 - 2099 )
* 入口参数: c_flag:阳历的世纪标志  clock_time: 时钟地址
* 出口参数: 无
* 说明: c_flag = 0 :21世纪 c_flag = 1 :19世纪 
*****************************************************************************/
void Calendar_Convert( uchar c_flag, uchar * clock_time )
{
   bit flag_month, flag_year;
   uchar year, month, day, month_point; //定义 年 月 天
   uchar temp1, temp2, temp3;
   uint calendar_address; //定义农历地址
   uint day_number;
   uchar clock_moon[3]; //定义阴历


   clock_time += 3; //指向日
   day = ( * clock_time >> 4 ) * 10 + ( *clock_time & 0x0f ); //BCD转换十进制
   clock_time ++; //指向月
   month = ( * clock_time >> 4 ) * 10 + ( * clock_time & 0x0f ); //BCD转换十进制
   clock_time ++; //指向年
   year = ( * clock_time >> 4 ) * 10 + ( * clock_time & 0x0f ); //BCD转换十进制 
  
   //定位日历地址
   if( c_flag == 0 )
      calendar_address = ( year + 99 ) * 3;  
   else
      calendar_address = ( year - 1 ) * 3;

   //春节(正月初一)所在的阳历月份
   temp1 = year_code[ calendar_address + 2 ] & 0x60; //Bit6~~Bit5:春节所在的阳历月份
   temp1 >>= 5 ;
   //春节(正月初一)所在的阳历日期
   temp2 = year_code[ calendar_address + 2 ] & 0x1f; //Bit4~~Bit0:春节所在的阳历日期
  
   //计算春节(正月初一)离当年元旦{ 1月1日(阳历) }的天数；春节只会在阳历的1月 或 2月
   /*if( temp1 == 1 )
      temp3 = temp2 - 1;
   else
      temp3 = temp2 + 31 - 1;
   	 */

	temp3=temp2-1;
	if(temp1!=1) temp3+=0x1f;
   

   //计算阳历月离当年元旦{ 1月1日(阳历) }的天数
   if( month < 10 )
      {day_number = day_code1[ month - 1 ] + day  ;} 
   else
      {day_number = day_code2[ month - 10 ] + day ;}

   //如果阳历的月大于2 且该年的2月为闰月，天数加1 
   //闰年指的就是阳历有闰日或阴历有闰月的一年；
   //阳历四年一闰，在二月加一天，这一天叫做闰日：
   //农历三年一闰，五年两闰，十九年七闰，每逢闰年所加的一个月叫做闰月。
   if( ( month <= 2 ) || ( year % 0x04!= 0) ) day_number-=1;
   //   day_number ++;

  //   if ((month<2)||(year%0x04!=0))
  //   day_number-=1;

   //判断阳历日 在春节(正月初一) 之前 还是 之后
   if( day_number >= temp3 ) //阳历在春节之后 或者 春节当日
   {
      day_number -= temp3;
      month = 1;
      month_point = 1; // month_point 为月份指向，阳历日在春季前就是春季
	  flag_month = get_moon_day( month_point, calendar_address ); //检查该阴历月的大小  大月返回1  小月返回0
	  flag_year = 0;
	 /* if( flag_month )
	     temp1 = 30; //大月30天
	  else
 	     temp1 = 29; //小月29天
      */
      if (flag_month==0) {temp1=29;} else{temp1=30;}

	  //闰月所在的月分
	  temp2 = year_code[ calendar_address ] & 0xf0;
	  temp2 >>= 4; //提取高四位  假如是0 表示没有闰月

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
   else //阳历在春节之前使用以下代码进行运算
   {
      temp3 -= day_number;
      if( year == 0 )
         { year = 0xe3; c_flag = 1; }
      else
         year -= 1;
      calendar_address -= 3;
      month = 0xc;
      temp2 = year_code[ calendar_address ] & 0xf0;
      temp2 >>= 4; //提取高4位
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
   //HEX->BCD ,运算结束后,把数据转换为BCD数据
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
* 名称: void Week_convert( uchar * clock_time )
* 功能: 输入BCD阳历数据；输出BCD星期
* 入口参数: 
* 出口参数:
* 说明: 算法: （ 日期 + 年份 + 所过闰年 + 月校正 ) / 7 的余数就是星期 如果是闰年又不到 3 月份上述之和 要减一天 再
********************************************************************************/
void Week_Convert( bit c, uchar * clock_time )
{
   uchar year, month, day; //定义 年 月 天
   uchar temp;

   clock_time += 3; //指向日
   day = ( * clock_time >> 4 ) * 10 + ( *clock_time & 0x0f ); //BCD转换十进制
   clock_time ++; //指向月
   month = ( * clock_time >> 4 ) * 10 + ( * clock_time & 0x0f );
   clock_time ++; //指向年
   year = ( * clock_time >> 4 ) * 10 + ( * clock_time & 0x0f ); 

   if( c == 0 ) //如果为21世纪,年份数加100
      year += 100;
   temp = year / 4; //所过闰年数只算1900年之后的
   temp = year + temp;
   temp = temp % 0x07; //为节省资源,先进行一次取余,避免数大于0xff,避免使用整型数据
   temp = temp + day + table_week[ month - 1 ];
   if( ( year % 4  == 0 ) && ( month <3  ) )
      temp -=1;
   Lcd_Week( temp % 7 );
}
/********************************************************************************/
#endif
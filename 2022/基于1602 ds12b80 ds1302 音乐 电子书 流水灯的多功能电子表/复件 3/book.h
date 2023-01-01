#include<reg52.h>
#include<lcd.h>
void detect();
void page(uchar k);
uchar i;
void main()
{
	ini();
	page(0);
	while(1)
	{
		detect();
	}	
}

void detect()
{
	uchar temp,key;
	temp=P1;
	temp=temp&0x0f;
	if(temp!=0x0f)
	{
		switch(temp)
		{
			case 0x0e:
			if(key!=0)
			{
				key--;
			}
			while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
			write_order(0x01);
			page(key);

			break;
			case 0x0d:
			key++;
			while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
			write_order(0x01);
			page(key);
			break;
		}
	}	
}

void page(uchar k)
{
	write_order(0x80);
	for(i=k*32;i<k*32+16;i++)
	{
		write_date(str[i]);
	}
	write_order(0x80+0x40);
	for(i=k*32+16;i<k*32+32;i++)
	{
		write_date(str[i]);
	}
}

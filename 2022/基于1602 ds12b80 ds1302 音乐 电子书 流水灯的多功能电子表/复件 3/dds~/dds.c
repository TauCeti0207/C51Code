#include<reg52.h>
#include<lcd.h>
#include<1302.h>
#include<music_code.h>
#include<SoundPlay.h>
#include<dds.h>
uchar code time[]={'0','1','2','3','4','5','6','7','8','9'};
uchar code get_up[]="Time to Get up!";
uchar code str[]="Never frown,even when you are sad,because you never know who is falling in love with your smile! To the world you may be one person,but to one person you may be the world ";
uchar code menu[4][10]={{"Time Set"},{"Music"},{"Books"},{"Ring Set"}};
void main()
{
	ini();
	open();
	delay(255);
	delay(255);
	delay(255);
	delay(255);
	write_order(0x01);
	init_ds1302();
	set_ds1302(time_add,initial_time);
	while(1)
	{
		read_ring();
		if(ring_time[0]==now_time[0]&&ring_time[1]==now_time[1]&&ring_time[2]==now_time[2])
		{
			timeup();
		}
		detect();
		write_order(0x80+0x04);		
		for(i=0;menu[key][i]!=0;i++)
		{
			write_date(menu[key][i]);
		}
		get_ds1302();
		write_order(0x80+0x40);
		write_date(time[now_time[4]>>4]);
		write_date(time[now_time[4]&0x0f]);
		write_date(0x01);
		write_date(time[now_time[3]>>4]);
		write_date(time[now_time[3]&0x0f]);
		write_date(0x00);
		write_date(' ');
		write_date(' ');
		write_date(time[now_time[2]>>4]);
		write_date(time[now_time[2]&0x0f]);
		write_date(':');
		write_date(time[now_time[1]>>4]);
		write_date(time[now_time[1]&0x0f]);
		write_date(':');
		write_date(time[now_time[0]>>4]);
		write_date(time[now_time[0]&0x0f]);
		if(key==2)
		{
		
			sum=0;
			book();

		}
		if(key==1)
		{
			music();

		}
		if(key==3)
		{		
			alamn();
	
		}
		if(key==0)
		{	
			time_set();
	
		}
	}
}
void detect()
{
	uchar temp;
	temp=P1;
	temp=temp&0x0f;
	if(temp!=0x0f)
	{
		switch(temp)
		{
			case 0x0e:
				while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
			if(key==0)
			{
				key=4;
			}
			key--;
			flag=1;
			write_order(0x01);
			break;
			case 0x0d:
				while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
			key++;
			flag=2;
			if(key==4)
			{
				key=0;
			}
			write_order(0x01);
			break;
			case 0x0b:
				while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
			flag=3;
			write_order(0x01);
			break;
			case 0x07:
				while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
			flag=4;
			write_order(0x01);
			break;
		}
	}

}
void page(uchar k)
{
	write_order(0x80);
	for(i=k*32;i<k*32+16&&str[i]!='\0';i++)
	{
		write_date(str[i]);
	}
	write_order(0x80+0x40);
	for(i=k*32+16;i<k*32+32&&str[i]!='\0';i++)
	{
		write_date(str[i]);
	}
}

void book()
{			
	
			flag=0;
			detect();
			if(flag==3)
			{
			flag=0;
			page(sum);
			while(flag!=4)
			{
				detect();
				if(flag==1)
				{
					flag=0;
					if(sum!=0)
					{
					sum--;
					}
					page(sum);
				}
				if(flag==2)
				{
					flag=0;
					if(sum<5)
					{
						sum++;
					}
					page(sum);
				}
			}
			key=2;
			}
}

void alamn()
{
			flag=0;
			detect();
			if(flag==3)
			{
				flag=0;
				while(flag!=4)
				{					
					write_order(0x80+0x02);
					write_date(time[move[0]]);
					write_date(time[move[1]]);
					write_date(':');
					write_date(time[move[2]]);
					write_date(time[move[3]]);
					detect();
					if(flag==1)
					{
						flag=0;
						move[1]++;
						
						if(move[1]==10)
							{
								move[1]=0;
								move[0]++;
							}
						
						if(move[0]==2)
						{
							if(move[1]==4)
							{
								move[1]=0;
								move[0]=0;
							}
						}
						ringgo[2]=move[0]<<4|move[1];
					}
					if(flag==2)
					{
						flag=0;
						move[3]++;
						if(move[3]==10)
						{
							move[3]=0;
							move[2]++;
							if(move[2]==6)
							{
								move[2]=0;
							}
						}
						ringgo[1]=move[2]<<4|move[3];
					}
					if(flag==3)
					{
						set_ds1302(alamn_add,ringgo);
						flag=4;
					}
					
				}
				key=3;
			}
}

void time_set()
{
			flag=0;
			detect();
			if(flag==3)
			{
				flag=0;
				while(flag!=4)
				{					
					write_order(0x80+0x02);
					write_date(time[set[0]]);
					write_date(time[set[1]]);
					write_date(':');
					write_date(time[set[2]]);
					write_date(time[set[3]]);
					detect();
					if(flag==1)
					{
						flag=0;
						set[1]++;
						
							if(set[1]==10)
							{
								set[1]=0;
								set[0]++;
							}
						
						if(set[0]==2)
						{
							if(set[1]==4)
							{
								set[1]=0;
								set[0]=0;
							}
						}
						timego[2]=set[0]<<4|set[1];
					}
					if(flag==2)
					{
						flag=0;
						set[3]++;
						if(set[3]==10)
						{
							set[3]=0;
							set[2]++;
							if(set[2]==6)
							{
								set[2]=0;
							}
						}
						timego[1]=set[2]<<4|set[3];
					}
					if(flag==3)
				{
					set_ds1302(time_add,timego);
					flag=4;
				}
				}
				key=0;	
			}
}

void timeup()
{	
			write_order(0x01);
			write_order(0x80+0x00);
			for(i=0;get_up[i]!='\0';i++)
			{
				write_date(get_up[i]);
			}
	
				InitialSound();
				switch(ring_song)
						{
							case 0:Play(tale,0,3,300);break;
							case 1:Play(coral,0,3,300);break;
							case 2:Play(Meet,0,3,300);break;
							case 3:Play(Love,0,3,340);break;
						}
				write_order(0x01);
}

void music()
{
	if(flag==3)
			{
				flag=0;	
				write_order(0x01);							
				while(flag!=4)
				{
						detect();
						write_order(0x80);
						for(i=0;song[key][i]!='\0';i++)
					{						
						write_date(song[key][i]);
					}
					if(flag==3)
					{
						InitialSound();
						switch(key)
						{
							case 0:Play(tale,0,3,300);break;
							case 1:Play(coral,0,3,300);break;
							case 2:Play(Meet,0,3,300);break;
							case 3:Play(Love,0,3,340);break;
						}
						ring_song=key;
						while(!esc);
					}                             
				}
				write_order(0x01);
				key=1;	
			}
}

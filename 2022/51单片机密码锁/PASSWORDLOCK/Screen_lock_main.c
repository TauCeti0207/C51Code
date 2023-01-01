#include <reg52.h>
#include <intrins.h>

#include "lxl_24C02C.h"
#include "uart.h"
#include "LCD1602.h"
#include <keypad44.h>

sbit greenled=P2^3;						//Enter the right passwords,green led will lighting.
sbit lock=P2^4;								//Enter the right passwords,electronic lock will unlock.
sbit redled=P2^5;							//Enter the wrong passwords,red led will lighting.
void show_base_infromation()	//show something personal information on the screen of LCD1206.just delay for a moment than cleanning all screen.
{
	uchar i;
	LCMDisplayString(0,0,"Design:LiXinLong");
	LCMDisplayString(1,0,"My ID:208171421");	
	for(i=0;i<4;i++)keypad_delay(65535);		//delay for a moment
	LCMClear();															//clean all screen.
}

void set_init_password()			//set users' initialize passwords. 
{
	Master_writeByte(1,1);			//The user 1 ,password:0001,password saved in register 1 of 24C02C.
	Master_writeByte(2,2);			//The user 2 ,password:0002,password saved in register 2 of 24C02C.
	Master_writeByte(3,3);			//The user 3 ,password:0003,password saved in register 3 of 24C02C.
}
int get_user_password(uchar i)//Get users' password ,i is the register number of the specific user.
{
	int read=Master_readByte(i);//read the appointed register.
	return _cror_(read,8);			//translated the value to int,and return the value.
}
void main()
{
	
	uchar presstimes=0;					//the register keep user's press times,maximum are 4,because the munber of password bits are defined as 4.  
	uchar enter_times=0;				//if user enter wrong password ,the register keep user's press times of "Enter",maximum are 3,beyond 3 account will locked(forbid to enter any number.).
	bit changepswd_state=0;			//flage bit of user pressed "Reset password" key,1:pressed,0:not pressed.
	bit change_use1=0;					//flage bit of user1 entered the old password correctly,and input the new passwords done.
	bit change_use2=0;					//*****************************similar to change_use1***********************************
	bit change_use3=0;					//*****************************similar to change_use1***********************************
	bit beyond3times=1;					//flage bit of users' entered the wrong password beyond 3 times.1: not beyond,0:beyond.

	
	int readkeypad;														//the register keep the keypad return value.
	int keypadvalue=0;												//the register keep the value when 4 times pressed.
	int press1,press2,press3,press4;					//press1:the value of 1st keypad return value multiple oppointed scale.press2-3 are similar to this.
	UartInit();																//initialize the USART for printf() to debug.
	LCMInit();																//initialize the LCD1206.
	printf("\nshow some \npersonal \ninfromation!\n");
	show_base_infromation();
	printf("\nplease enter your\npassword\n");
	keypad_delay(65535);	
	set_init_password();
	LCMDisplayString(0,0,"Input pswd:");
	LCMDisplayString(1,0,"You error 0 Time");
	
	while(1)
	{
		readkeypad=key_scan();
		if(((readkeypad<10)||(readkeypad==14)||(readkeypad==13))&&beyond3times)//allowed press 0-9,"clean" or "enter" when account not lock(wrong password not beyond 3 times).
		{
			presstimes=presstimes+1;
			switch(presstimes)
			{
				case 1:															//the 1st bit of 4bits password.
					{
						press1=readkeypad*1000;	
						LCMDisplayString(0,11,"*");		
						printf("pressed %d\n",readkeypad);
					}break;
				case 2:															//the 2nd bit of 4bits password.
					{
						press2=readkeypad*100;
						LCMDisplayString(0,11,"**");
						printf("pressed %d\n",readkeypad);
					}break;
				case 3:															//the 3rd bit of 4bits password.
					{
						press3=readkeypad*10;
						LCMDisplayString(0,11,"***");
						printf("pressed %d\n",readkeypad);
					}break;
				case 4:															//the 4th bit of 4bits password.
					{
						press4=readkeypad;
						LCMDisplayString(0,11,"****");
						printf("pressed %d\n\n",readkeypad);
					}break;
				default:break;				
			}			
		}
		
		if((presstimes<=5)&&(readkeypad>11))		//Entered 4bits password,checking whether "Enter" or "Clean" pressed.
			{
				switch(readkeypad)
				{
					case 14:													//pressed 'Enter' key
					{
						keypadvalue=press1+press2+press3+press4;
						presstimes=0;
						if(enter_times<3)LCMDisplayString(0,11,"    ");
						if(change_use1)									
							{
								Master_writeByte(1,keypadvalue);
								change_use1=0;
								LCMClear();
								LCMDisplayString(0,0,"Input pswd:");
								LCMDisplayString(1,0,"You error 0 Time");
							}
						if(change_use2)
							{
								Master_writeByte(2,keypadvalue);
								change_use2=0;LCMClear();
								LCMDisplayString(0,0,"Input pswd:");
								LCMDisplayString(1,0,"You error 0 Time");
							}
						if(change_use3)
							{
								Master_writeByte(3,keypadvalue);
								change_use3=0;
								LCMClear();
								LCMDisplayString(0,0,"Input pswd:");
								LCMDisplayString(1,0,"You error 0 Time");
							}//printf("pswd changed!\n");
						
						if((keypadvalue==get_user_password(1))||(keypadvalue==get_user_password(2))||(keypadvalue==get_user_password(3)))//matching password saved in 24C02C,only password are correct can match successful.
						{
							greenled=0;
							lock=0;
							redled=1;
							printf("\nWelcome You to\nGo Home!\n");
							enter_times=0;
							LCMDisplayChar(1,10,'0');
						}else														//match password failed.
						{
							greenled=1;
							lock=1;
							redled=0;
							printf("\nPasswords error\nplease try\nagain!\n");
							enter_times++;
							switch(enter_times)						//show times of enter wrong password.
							{
								case 0:LCMDisplayChar(1,10,'0');break;
								case 1:LCMDisplayChar(1,10,'1');break;
								case 2:LCMDisplayChar(1,10,'2');break;
								default:										//enter wrong password 3 times,lock the account!
									{
										beyond3times=0;
										LCMClear();
										LCMDisplayString(0,0,"Account Locked!");
										LCMDisplayString(1,0,"Call 1401856153");
										printf("\nInput error\nbeyond 3 times\nthe account\nlocked!\nplease Contact \nadministrator\n");
									}break;
							}
						}
					}break;
					case 13:													//press 'Clean' key,clean all entered numbers.
					{
						presstimes=0;
						LCMDisplayString(0,11,"    ");
						greenled=1;
						lock=1;
						printf("\nPressed Clean!\n");
						
					}break;
					default:break;
				}
				
			}
			
			if((presstimes==5)&&(readkeypad<10))//if 5th entered is a number of 0-9.clean all entered passwords,because passwords were designed 4bits.
			{				
				presstimes=0;
				LCMDisplayString(0,11,"    ");
				printf("\nPlease Reenter Your\nPassword\n");
			}
			
			if(readkeypad==12)									//Press "reset password" key,enter to reset passwords process while loop.
			{
				changepswd_state=1;
				LCMClear();
				LCMDisplayString(0,0,"Enter Old pswd:");
				printf("\nEnter your old\npswd now:\n");
			}else changepswd_state=0;
			
			while(changepswd_state)							//Reset passwords while loop
			{
				readkeypad=key_scan();
					if((readkeypad<10)||(readkeypad==14)||(readkeypad==13))
					{
						presstimes=presstimes+1;
						switch(presstimes)
						{
							case 1:	
								{
									press1=readkeypad*1000;	
									LCMDisplayString(1,0,"*");		
									printf("pressed %d\n",readkeypad);
								}break;
							case 2:
								{
									press2=readkeypad*100;		
									LCMDisplayString(1,0,"**");		
									printf("pressed %d\n",readkeypad);
								}break;
							case 3:
								{
									press3=readkeypad*10;		
									LCMDisplayString(1,0,"***");	
									printf("pressed %d\n",readkeypad);
								}break;
							case 4:	
								{
									press4=readkeypad;				
									LCMDisplayString(1,0,"****");	
									printf("pressed %d\n\n",readkeypad);
								}break;
							default:break;				
						}			
					}
					
					if((presstimes<=5)&&(readkeypad>11))
					{
						switch(readkeypad)
						{
							case 14:										//press 'enter' key
								{
										keypadvalue=press1+press2+press3+press4;
										presstimes=0;
										LCMDisplayString(1,0,"    ");
										if(keypadvalue==get_user_password(1))				//matched user1's passwords.
										{
											LCMClear();
											LCMDisplayString(1,0,"Enter New pswd:");
											presstimes=0;
											change_use1=1;
											changepswd_state=0;
										}else if(keypadvalue==get_user_password(2))	//matched user2's passwords.
										{
											LCMClear();
											LCMDisplayString(1,0,"Enter New pswd:");
											presstimes=0;
											change_use2=1;
											changepswd_state=0;
										}else if(keypadvalue==get_user_password(3))	//matched user3's passwords.
										{
											LCMClear();
											LCMDisplayString(1,0,"Enter New pswd:");
											presstimes=0;
											change_use3=1;
											changepswd_state=0;
										}else																				//Didn't match user1's passwords.
										{
											presstimes=0;
											changepswd_state=0;
											change_use1=0;
											change_use2=0;
											change_use3=0;
											
											printf("\nOld Pswd Error!\n");
											LCMClear();
											LCMDisplayString(0,0,"Input pswd:");
											LCMDisplayString(1,0,"You error   Time");
										}										
								}break;					
							default:break;
						}				
					}
				LCMDelay(1000);																					//keypad scan delay time.
			}			
			LCMDelay(1000);																						//keypad scan delay time.
	}
	
}
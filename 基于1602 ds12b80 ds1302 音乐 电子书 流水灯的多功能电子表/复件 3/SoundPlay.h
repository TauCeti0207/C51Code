
#ifndef __SOUNDPLAY_H_REVISION_FIRST__
#define __SOUNDPLAY_H_REVISION_FIRST__
#include<dds.h>
//#include<intrins.h>
#include<lcd.h>
#define SYSTEM_OSC 		12000000
#define SOUND_SPACE 	4/5 		
sbit    BeepIO = P3^5;
sbit    esc=P2^3;
sbit    pause=P2^2;		
uint  code FreTab[12]  = { 262,277,294,311,330,349,369,392,415,440,466,494 }; 
uchar code SignTab[7]  = { 0,2,4,5,7,9,11 }; 								 
uchar code LengthTab[7]= { 1,2,4,8,16,32,64 };						
uchar Sound_Temp_TH0,Sound_Temp_TL0;	
uchar Sound_Temp_TH1,Sound_Temp_TL1;	
uchar temp1=0xfe;
uchar bee,change;
void InitialSound(void)
{
	BeepIO = 0;
	Sound_Temp_TH1 = (65535-(1/1200)*SYSTEM_OSC)/256;	
	Sound_Temp_TL1 = (65535-(1/1200)*SYSTEM_OSC)%256;	
	TH1 = Sound_Temp_TH1;
	TL1 = Sound_Temp_TL1;
	TMOD  |= 0x11;
	ET0    = 1;
	ET1	   = 0;
	TR0	   = 0;
	TR1    = 0;
	EA     = 1;
	TMOD = 0xfd;
	   CP_RL2 = 0;
	   TL2 = 0xf0;
	   TH2 = 0xd8;
	   RCAP2L = 0xf0;
	   RCAP2H = 0xd8;
	   C_T2 = 0;
	   TR2 = 1;
		 ET2 = 1;
}

void BeepTimer0(void) interrupt 1
{
		BeepIO = !BeepIO;
		TH0    = Sound_Temp_TH0;
	 	TL0    = Sound_Temp_TL0;
		
	
}
void timer2() interrupt 5
{
	
	TF2=0;
	bee++;
	if(esc==0) flag=4;
	if(pause==0) flag=3;
	if(bee==100)
	{
			bee=0;
			change++;
			if(change<9)
			{
				P1=temp1;
				temp1=_crol_(temp1,1);					
			}
		
			if(change>=9&&change<17)
			{				
				temp1=_cror_(temp1,1);
				P1=temp1;				
			}
			if(change>=17&&change<25)
			{					
				temp1=temp1<<1;
				P1=temp1;				
			}
			if(change>=25&&change<32)
			{
				P1=~P1;
			}
			if(change==32)
			{
				change=0;
				temp1=0xfe;
			}
	}
	
}
void Play(uchar *Sound,uchar Signature,uchar Octachord,uint Speed)
{
	uint NewFreTab[12];		
	uchar i,j;
	uint Point,LDiv,LDiv0,LDiv1,LDiv2,LDiv4,CurrentFre,Temp_T,SoundLength;
	uchar Tone,Length,SL,SH,SM,SLen,XG,FD;
	
		for(i=0;i<12;i++) 				
		{
			j = i + Signature;
			if(j > 11)
			{
				j = j-12;
				NewFreTab[i] = FreTab[j]*2;
			}
			else
				NewFreTab[i] = FreTab[j];
	
			if(Octachord == 1)
				NewFreTab[i]>>=2;
			else if(Octachord == 3)
				NewFreTab[i]<<=2;
		}									
		
		SoundLength = 0;
		while(Sound[SoundLength] != 0x00)
		{
			SoundLength+=2;
		}
	
		Point = 0;
		Tone   = Sound[Point];	
		Length = Sound[Point+1]; 		
		
		LDiv0 = 12000/Speed;			
		LDiv4 = LDiv0/4; 					
		LDiv4 = LDiv4-LDiv4*SOUND_SPACE; 
		TR0	  = 0;
		TR1   = 1;
		while(Point < SoundLength)
		{
			
			SL=Tone%10; 							
			SM=Tone/10%10; 								
			SH=Tone/100; 							
			CurrentFre = NewFreTab[SignTab[SL-1]+SH]; 	
			if(SL!=0)
			{
				if (SM==1) CurrentFre >>= 2; 	
				if (SM==3) CurrentFre <<= 2; 		
				Temp_T = 65536-(50000/CurrentFre)*10/(12000000/SYSTEM_OSC);
				Sound_Temp_TH0 = Temp_T/256; 
				Sound_Temp_TL0 = Temp_T%256; 
				TH0 = Sound_Temp_TH0;  
				TL0 = Sound_Temp_TL0 + 12;
			}
			SLen=LengthTab[Length%10]; 	
			XG=Length/10%10; 		
			FD=Length/100;
			LDiv=LDiv0/SLen; 			
			if (FD==1) 
				LDiv=LDiv+LDiv/2;
			if(XG!=1)	
				if(XG==0) 				
					if (SLen<=4)	
						LDiv1=LDiv-LDiv4;
					else
						LDiv1=LDiv*SOUND_SPACE;
				else
					LDiv1=LDiv/2; 		
			else
				LDiv1=LDiv;
			if(SL==0) LDiv1=0;
				LDiv2=LDiv-LDiv1;
				
		  	if (SL!=0)
			{
				TR0=1;
				for(i=LDiv1;i>0;i--) 	
				{
					while(TF1==0);
					TH1 = Sound_Temp_TH1;
					TL1 = Sound_Temp_TL1;
					TF1=0;
				}
			}
			if(LDiv2!=0)
			{
				TR0=0; BeepIO=0;
				for(i=LDiv2;i>0;i--) 	
				{
					while(TF1==0);
					TH1 = Sound_Temp_TH1;
					TL1 = Sound_Temp_TL1;
					TF1=0;
				}
			}
			if(flag==3)
			{	
				
				EA=0;
				TR2=0;
				TR1=0;
				while(!pause);
				EA=1;
				TR2=1;
				TR1=1;
				flag=0;
			}
			if(flag==4)
			{
				flag=0;
				Point=SoundLength;
			}
			else
			{
				Point+=2; 
			}
			Tone=Sound[Point];
			Length=Sound[Point+1];
		}
		flag=0;
		BeepIO = 0;
		temp1=0xfe;
		P1=0xff;
		bee=0;
		change=0;
		TR2 = 0;
	   	ET2 = 0;
		EA=0;
		ET0=0;
		ET1=0;
		TR0=0;
		TR1=0;
	 
 
}
#endif
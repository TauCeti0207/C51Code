#include "uart.h"

//------------------------------------------------------------------------------------------
// fuction  : initialize the uart, we can config it in uart.h 
// parameter: no
// return   : no
//------------------------------------------------------------------------------------------
void UartInit(void)			
{
#if TIMER_FOR_UART == 1                  	//* use T1 to generate uart clock
	{
		TMOD = (TMOD & 0x0F) | 0x20;
	#if BPS >= 38400                     		//* if the bps excess 38400(include 38400)
		PCON|=0x80;                      			//* I set the SMOD bit
		#define SMOD_BIT 1
	#else
		PCON=0;
		#define SMOD_BIT 0
	#endif

	#if X2_MODE ==0                       	//* don't use X2 mode
		#if SMOD_BIT == 1
			TH1=256-FOSC/12/16/BPS;
		#elif SMOD_BIT == 0
			TH1=256-FOSC/12/32/BPS;
		#endif
		TL1=TH1;
	#elif X2_MODE ==1
		#if SMOD_BIT == 1
			TH1=256-FOSC/6/16/BPS;
		#elif SMOD_BIT == 0
			TH1=256-FOSC/6/32/BPS;
		#endif
		TL1=TH1;
	#endif
		SCON=0x50;                       			//* mode 1, allow to rx
		TR1=1;
		TI=1;
	}
#elif TIMER_FOR_UART ==2  								//* use T2 to generate uart clock
	{
	#if X2_MODE == 0
		RCAP2H=(65536-FOSC/32/BPS)/256;				//*12-clock mode 
		RCAP2L=(65536-FOSC/32/BPS)%256;
	#elif X2_MODE ==1
		RCAP2H=(65536-FOSC/16/BPS)/256;				//*6-clock mode 
		RCAP2L=(65536-FOSC/16/BPS)%256;
	#endif		
		SCON = 0x50;													//*USART MODE 1
		T2CON = 0x34;	                   
		TI=1;

	}
#endif
}
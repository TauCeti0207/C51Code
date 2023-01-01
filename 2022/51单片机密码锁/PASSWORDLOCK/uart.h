#ifndef __UART_H__
#define __UART_H__
#include <reg52.h>
#include <stdio.h>


/****************************** fuction prototype **********************************/
void UartInit(void);														    
							          
/****************************** uart configuation ***********************************/


/****************************** uart configuation ***********************************/
#ifndef FOSC
#	define FOSC			11059200   	/* the frequency of the crystal         		*/
#endif

#define BPS				9600	    	/* boundrate                                    */
#define X2_MODE			0	        /* this should be 1 if use X2 mode              */
#define TIMER_FOR_UART	1    	/* if use T1 to generate uart clock,this should */
                            	/* be 1, if use T2, this should be 2            */
/************************************************************************************/

/* Note : When use T2 to generate uart clock ,can't simulate in Keil C */
#endif
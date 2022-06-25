#include "keypad44.h"
 
char code keypadport_scan[]={0x7f,0xbf,0xdf,0xef}; 	//match key coding.
 
char code key_codes_value[]={
					0xee,0xed,0xeb,0xe7,
					0xde,0xdd,0xdb,0xd7,
					0xbe,0xbd,0xbb,0xb7,
					0x7e,0x7d,0x7b,0x77
};
void keypad_delay(unsigned int x)     							//keypad scan delay. 
{
	while(x--);
}


unsigned char key_scan()	 
{
	unsigned char i,j;
	for(i=0;i<4;i++)		
	{
		keypad_port=keypadport_scan[i];
		if(keypad_port!=keypadport_scan[i])
		{
			keypad_delay(5000);														//delay about 5ms,removing key shaking.
			if(keypad_port!=keypadport_scan[i])
			{
				for(j=0;j<16;j++)
				{
					if(keypad_port==key_codes_value[j])
					{
						switch(key_codes_value[j])							//coding keypad keys.
						{
							case 0xee: return 10;break;						//ON/C
							case 0xed: return 0; break;						//0
							case 0xeb: return 11;break;						//=
							case 0xe7: return 12;break;						//+
							case 0xde: return 1; break;						//1
							case 0xdd: return 2; break;						//2
							case 0xdb: return 3; break;						//3
							case 0xd7: return 13;break;						//-
							case 0xbe: return 4; break;						//4
							case 0xbd: return 5; break;						//5
							case 0xbb: return 6; break;						//6
							case 0xb7: return 14;break;						//X
							case 0x7e: return 7; break;						//7
							case 0x7d: return 8; break;						//8
							case 0x7b: return 9; break;						//9
							case 0x77: return 15;break;						///
							default:						 break;
						}
					}	  
				}
			}
		}
	}
	return(16);																				//without key press return 16.
}
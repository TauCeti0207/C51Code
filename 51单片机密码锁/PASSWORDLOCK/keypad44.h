#ifndef __KEYPAD44_H__
#define __KEYPAD44_H__

#include <reg52.H>
#define keypad_port P1

extern char code keypadport_scan[];
 
extern char code key_codes_value[];

void keypad_delay(unsigned int x);
unsigned char key_scan(void);


#endif

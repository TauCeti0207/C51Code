#ifndef __LXL_24C02C_H__
#define __LXL_24C02C_H__
#include <reg52.h>
#include <intrins.h>
sbit scl=P2^6;
sbit sda=P2^7;

#define iic_delay 1	/*modify the I2C comunication delay time to adapt special need,especially in protues simulation!!!*/
#define WRITE 0xa0	/*modify the value according to IC!!!*/
#define READ 0xa1		/*modify the value according to IC!!!*/

typedef unsigned int uint;
typedef unsigned char uchar;

void Master_writeByte(uchar addr,uchar Byte);
uchar Master_readByte(uchar addr);

void iic_delay_10us(uint multi);
void iic_start(void);
void iic_stop(void);
void iic_master_ack(void);
void iic_master_noack(void);
void Master_writeData(uchar masterWriteDate);
uchar Master_readData(void);
uchar check_ack(void);




#endif
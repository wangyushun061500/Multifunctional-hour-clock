#include "H Group/lm75a.h"
#include "H Group/config.h"
#include "base Group/i2c.h"

int lm75_get(void){
	unsigned char buf[2];
	int t;
	i2c_gets(0x90,0x00,2,buf);
	if(!(buf[0]&0x80))
		{
		t=buf[0];
		t=t<<8;
		t=((t|buf[1])>>5);
		}
     else
		{
		t=buf[0];
		t=t<<8;
		t=((t|buf[1])>>5);
		t=(0x800-t);
		}
	return t;
}
#endif
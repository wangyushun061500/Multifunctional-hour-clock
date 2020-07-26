#include "H Group/config.h"
#include "H Group/74hc164.h"

/**74HC164显示程序***/
void send(unsigned char a)
{
	unsigned char code led_data[] = {
	0x77,0x06,0x6b,0x2f,0x1e,0x3d,0x7d,0x07,0x7f,0x3f,//0~9,
	0x5f,0x7c,0x71,0x6e,0x79,0x59,0x5e,0x70,0x08,0x80,0x00
	//A(10)b(11)c(12)D(13)E(14)F(15),H(16),L(17),-(18),小数点(19),熄灭(20)
	};
	unsigned char i,j;
	all = 0;
	all = 1;
	j = led_data[a];
	for(i=0;i<8;i++){
		if((j&0x01) == 0){
			dat = 0;
			clk = 0;
			clk = 1;
		}else{
			dat = 1;
			clk = 0;
			clk = 1;
		}
		j = j>>1;
	}
}
#endif
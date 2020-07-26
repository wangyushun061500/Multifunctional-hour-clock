#include "H Group/config.h"
#include "H Group/display_led.h"
#include "H Group/74hc164.h"

unsigned int shine;
bit dp;

/***显示函数延时函数***/
void delay_display(unsigned int c)
{
	for(c;c>0;c--){
		_nop_();
	}
}
/***显示函数***/
void dis_led(unsigned char a,b)//两个输入值，分别是位数和字符数
{
	unsigned char led_num[] = {
	0x0e,0x0d,0x0b,0x07
	};
	all = 1;
	display = led_num[a];  //显示位
	send(b);//显示段码
	if(dp == 1){//判断小数点位是否开启了
		ddp = 0;
	}else{
		ddp = 1;
	}
	delay_display(shine);  //前延时，用来调节亮度的亮时长
	all = 0;//清除位显示
	display = ~led_num[a];
	ddp = 1;//关断冒号
	delay_display(1024-shine);  //后延时，用来调节亮度的灭时长
}
#endif
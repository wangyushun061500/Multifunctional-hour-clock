#include "H Group/config.h"
#include "H Group/lm75a.h"
#include "H Group/74hc164.h"
#include "H Group/display_led.h"
#include "base Group/adc.h"

extern u16 shine;
extern bit dp; 
bit TempF = 0,cor_bit = 0,space_bit = 0,off_bit = 0;
u8 ss,mm,hh;
u8 menu = 1,keyvalue,dis_set,cor_all;
u16 space,space_temp = 400,addr = 0;
int temperature;


void delayms(u16 c);//ms延时，自适应
void delayus(u16 c);//us延时
void timer0_init(void);//timer0初始化，20ms每次
void timer2_init(void);//timer2初始化，5ms每次
u16 adc_avg(void);//读取红外感应距离数值并且进行抗干扰算法
void KeyHandle(void);//对按键操作进行判断

void main(void){
	/***定义main函数内变量***/
	/***引脚初始化***/
	gled = wled = 1;
	g1 = g2 = g3 = g4 = 1;
	key1 = key2 = key3 = 1;
	gled = 0;
	delayms(100);
	gled = 1;
	wled = 0;
	/***变量初始化***/
	hh = 11;
	mm = 1;
	ss = 0;
	/***程序函数初始化***/
	timer0_init();
	timer2_init();
	adc_init();
	/***特殊寄存器初始化***/
	EA = 1;//开启总中断
	ET0 = 1;//允许定时器0中断
	PT0 = 1;//定时器0中断优先级最高
	IE2 = 0x04;//允许定时2产生中断
	delayms(100);//延时等待程序稳定
	/***抗干扰初始化***/
	space_temp = space;//重读一次抗干扰数据
	/***正式程序***/
	while(1){//主循环
		KeyHandle();
	}
}
/**1ms延时程序**/
void delayms(u16 c)
{
	u8 i,j;
	for(c;c>0;c--){
		_nop_();
		_nop_();
		i = 22;
		j = 128;
		do
		{
			while (--j);
		} while (--i);
	}
}
/**1us延时程序**/
void delayus(u16 c)
{
	u8 i;
	for(c;c>0;c--){
		i = 3;
		while (--i);
	}
}

/**定时器T0初始化**/
void timer0_init(void)//20ms
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x70;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void timer2_init(void)		//5ms
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x00;		//设置定时初值
	T2H = 0xDC;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
}

u16 adc_avg(void)
{
	u16 shine_led,night_led;
	if(space_bit == 0){
		o_led = 1;
		wled = 1;
		delayus(200);
		night_led = get_adc(0);
		o_led = 0;
		wled = 0;
		delayus(40);
		shine_led = get_adc(0);
		o_led = 1;
		wled = 1;
	}
	return (shine_led - night_led);
}
/**按键判断程序**/
void KeyHandle(void)
{
	if((~key1 || ~key2 || ~key3) && (keyvalue == 0)){//判断按键操作标志，如果操作标志为未操作状态，就执行单个按键确认
		if(~key1){
			keyvalue = 1;//置位相应的值
			while(~key1);//按键松开才允许继续运行
		}else if(~key2){
			keyvalue = 2;//置位相应的值
			while(~key2);
		}else if(~key3){
			keyvalue = 3;//置位相应的值
			while(~key3);
		}
	}
		if(keyvalue == 1){//如果为第一个操作状态
			menu = 1;
			if(dis_set == 0){//判断是否为空，并且进行初始化
				dis_set = 1;//默认为第一个操作按键
			}
			if((~key1) && (dis_set != 0)){ //如果进行了初始化，并且按键有效
				dis_set++;//响应按键加一
				if(dis_set == 5){//每4个位一轮
					dis_set = 1;//每次加一，不进行0（空状态跳转）
				}
				while(~key1);//进行按键等待
			}
			if((~key2) && (dis_set != 0)){//如果按键2有效
				switch(dis_set){//判断上面的位设置值
					case 1: hh = hh+10;break;//每十个加一
					case 2: hh++;break;//自加一
					case 3: mm = mm+10;break;
					case 4: mm++;break;
					default: break;//没有操作自行退出
				}
				while(~key2);//判断完成再进行等待
			}
			if(~key3){//确认按键
				dis_set = 0;//复位位设置位为空
				keyvalue = 0;//复位当前设置标志为空
				while(~key3);//进行等待
				menu = 1;
			}
		}
		if(keyvalue == 2){
			menu = 2;
			if(~key1 || ~key2){
				TempF = ~TempF;
				while(~key1 || ~key2);//按键1和按键2都可以进行修改
			}
			if(~key3){//确认按键
				keyvalue = 0;//复位当前设置标志为空
				while(~key3);//进行等待
				menu = 2;
			}
		}
		if(keyvalue == 3){
			menu = 3;
			if(~key1){
				if(cor_all == 0){//先判断防止出现负值
					cor_all = 3;
				}else{
					cor_all--;
					if(cor_all == 0){//先判断防止出现负值
					cor_all = 4;
					}
				}
				while(~key1);
			}
			if(~key2){
				cor_all++;
				if(cor_all == 5){
					cor_all = 1;
				}
				while(~key2);
			}
			if(~key3){//确认按键
				keyvalue = 0;//复位当前设置标志为空
				switch(cor_all){
					case 1:{space_bit = 1; cor_bit = 0; off_bit = 1;};break;
					case 2:{space_bit = 1; cor_bit = 1; off_bit = 0;};break;
					case 3:{space_bit = 0; cor_bit = 0; off_bit = 0;};break;
					case 4:{space_temp = space;};break;
					default: cor_all = 3;break;
				}
				while(~key3);//进行等待
				menu = 1;//自动关断
			}
		}
	
}
/**T0中断程序**/
void timer0(void) interrupt 1 
{
	u8 time,time1;
/**总时间判断**/
	time++;
	if((menu != 0) && (keyvalue == 0) && (off_bit == 0)){//自动关断判断
		time1++;
		if(time1 == 250){//判断循环位是否为空
			if(cor_bit == 0){
				menu = 0;
			}else{
				if(menu == 1){//每五秒切换一个状态
					menu = 2;
				}else{
					menu = 1;
				}
			}
		}
	}
	/**小数点判断**/
	if((time > 25) && (menu == 1)){
		dp = (bit)1;
	}else{
		dp = (bit)0;
	}
	if(time == 50){//每一个s
		time = 0;
		ss++;//自加
		if(ss == 60){//分钟加一
			ss = 0;
			mm++;
			if(mm == 60){//小时加一
				mm = 0;
				hh++;
				if(hh == 24){//凌晨复位
					hh = 0;//24小时制
				}
			}
		}
	}
	if(ss > 59){//大于59自动复位，
		ss = 0;//调节时间不进行关联相加
	}
	if(mm > 59){
		mm = 0;
	}
	if(hh > 23){
		hh = 0;
	}
}
/***T2中断程序***/
void timer2_isr(void) interrupt 12 
{
	bit time_out;
	u8 count1,count2,timer,t;
	u16 Temp;
	timer++;
	if(timer == 10){
		timer = 0;
		t++;
		if(t == 10){
			t = 0;
			if(menu == 2){
				temperature = lm75_get();//读取温度
				temperature = temperature*125;
			}
		}
		/*读取adc数据*/
		space = adc_avg();//读取距离
		shine = get_adc(1);//读取光照度信息
		
		if(((hh > 6) && (mm > 30)) && ((hh < 18) && (mm <30)) && (shine > 500)){
			shine = shine + 200;
		}
		else if(shine < 200){
			wled = 0;
		};
		
		if(shine > 1010)shine = 1010;
		if(shine < 100)shine = 100;
		/*感应开关抗干扰*/
		if(space_bit == 0){
			if(space > (space_temp+20)){//开启距离
				count1++;
				count2 = 0;
				if(count1 == 3){
					count1 = 0;
					if(time_out == 0){
						if(menu == 1){
							menu = 2;
						}else{
							menu = 1;
						}
						time_out = 1;
					}
				}
			}
			if(space < (space_temp+10)){//关闭距离
				count2++;
				count1 = 0;
				if(count2 == 3){
					count2 = 0;
					time_out = 0;
				}
			}		
		}
	}
	if(menu == 1){//时钟显示
		if(keyvalue == 1){//独立于定时器里面，防止主程序的按键判断等待的时候出现不显示的问题
			if(dis_set == 1){//显示位1
				if(~dp){
					dis_led(0,hh/10);
				}else{
					dis_led(0,20);
				}
			}else{
				dis_led(0,hh/10);
			}
			
			if(dis_set == 2){//显示位2
				if(~dp){
					dis_led(1,hh%10);
				}else{
					dis_led(0,20);
				}
			}else{
				dis_led(1,hh%10);
			}
			
			if(dis_set == 3){//显示位3
				if(~dp){
					dis_led(2,mm/10);
				}else{
					dis_led(0,20);
				}
			}else{
				dis_led(2,mm/10);
			}
			
			if(dis_set == 4){//显示位4
				if(~dp){
					dis_led(3,mm%10);
				}else{
					dis_led(0,20);
				}
			}else{
				dis_led(3,mm%10);
			}
		}else{//正常显示
			dis_led(0,hh/10);
			dis_led(1,hh%10);
			dis_led(2,mm/10);
			dis_led(3,mm%10);
		}
	}
	if(menu == 2){//温度显示
		if(TempF == 1){//显示为华摄氏度和摄氏度的标志位
			Temp = ((temperature/100)*18/100)+32;//无符号所以多进行了一次整数运算
			dis_led(0,Temp/100);
			dis_led(1,Temp%100/10);
			dis_led(2,Temp%10);
			dis_led(3,15);
		}else{
			dis_led(0,temperature/10000);
			dis_led(1,temperature%10000/1000);
			dis_led(1,19);
			dis_led(2,temperature%1000/100);
			dis_led(3,12);
		}
	}
	if(menu == 3){//显示all
		if(cor_all != 0){
		switch(cor_all){
			case 1: {dis_led(0,18);dis_led(1,0);dis_led(2,1);dis_led(3,18);};break;
			case 2: {dis_led(0,18);dis_led(1,0);dis_led(2,2);dis_led(3,18);};break;
			case 3: {dis_led(0,18);dis_led(1,0);dis_led(2,3);dis_led(3,18);};break;
			case 4: {
				dis_led(0,space_temp/1000);
				dis_led(1,(space_temp%1000)/100);
				dis_led(2,(space_temp%100)/10);
				dis_led(3,space_temp%10);
			}
			default: break;
		}
	}else{
		dis_led(0,20);
		dis_led(1,10);
		dis_led(2,17);
		dis_led(3,17);
		}
	}
}
#endif
/*===========================冗余程序存放================================*/
//test测试程序
/*
		dis_led(0,space/1000);
		dis_led(1,(space%1000)/100);
		dis_led(2,(space%100)/10);
		dis_led(3,space%10);*/

/*	
		dis_led(0,shine/1000);
		dis_led(1,(shine%1000)/100);
		dis_led(2,(shine%100)/10);
		dis_led(3,shine%10);*/
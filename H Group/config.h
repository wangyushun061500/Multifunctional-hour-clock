#ifndef __CONFIG_H
#define __CONFIG_H

#include "15w.h"
#include "intrins.h"

/***管脚定义区***/
/**指示led灯**/
#define wled P36
#define gled P37
/**光敏电阻**/
#define rg1 P11
/**感应红外对管**/
#define i_led P10
#define o_led P35
/**数据通讯**/
/*74HC164*/
#define clk P25
#define dat P24
#define all P55
/*LM75**/
#define sda P26
#define scl P27
/***数码管定义***/
#define g1 P20
#define g2 P21
#define g3 P22
#define g4 P23
#define display P2
#define ddp P34
/***按键定义***/
#define key1 P32
#define key2 P33
#define key3 P54
/***发送数据管脚定义***/
#define p_txd P31
/***ADC定义***/
#define adc ADC_CONTR  //adc总控制
#define adc_datah ADC_RES  //adc转换输出高8位
#define adc_datal ADC_RESL  //adc转换输出低8位
#define adc_power 0x80 //因为adc_contr不允许位寻址，所以就要
#define adc_flag 0x10  //将对应每个位的开关写上，然后再将写入
#define adc_start 0x08 //的时候每个值相或（加入哪个变哪个）
#define adc_speedh 0x60
#define adc_speedl 0x00

#define fosc 22118400L
#define MAIN_Fosc		22118400L	//定义主时钟	 220 ~ 19200

#if (MAIN_Fosc >= 24000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_30MHZ
#elif (MAIN_Fosc >= 20000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_24MHZ
#elif (MAIN_Fosc >= 12000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_20MHZ
#elif (MAIN_Fosc >= 6000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_12MHZ
#elif (MAIN_Fosc >= 3000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_6MHZ
#elif (MAIN_Fosc >= 2000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_3MHZ
#elif (MAIN_Fosc >= 1000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_2MHZ
#else
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_1MHZ
	
#endif
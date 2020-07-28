#include "H Group/config.h"
#include "adc.h"

/***adc初始化函数***/
void adc_init(void)
{
	P2M1 = 0x00;
	P2M0 = 0x02;
	P1ASF = 0x03;//允许P1的1.0，1.1为输入位
	adc_datah = adc_datal = 0;  //清除缓存
	adc  = adc_power |adc_speedh;
  //延时用来成功输出数值
}
/**adc根据管脚数值取样**/
unsigned int get_adc(unsigned char channel)
{
	unsigned char i;
	adc_datah = adc_datal = 0;
	adc = (adc & 0xe0) | adc_start | channel;
	for(i=0; i<250; i++){
		if(adc & adc_flag)
		{
			adc &= ~adc_flag;
			return(adc_datah<<2 | adc_datal);
		}
	}
	return 1024;
}
#endif
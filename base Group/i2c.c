#include "H Group/config.h"
#include "i2c.h"

/**i2c延时函数**/
void delay(void)//delay
{
	char i = 2;
	while(i--);
}
/**i2c初始化**/
void i2c_init(void)
{
	scl = 1;
	sda = 1;
	delay();
	delay();
}
/**i2c开始信号**/
void i2c_start(void)
{
	sda = 1;
	delay();
	scl = 1;
	delay();
	sda = 0;
	delay();
	scl = 0;
	delay();
}
/**i2c终止信号**/
void i2c_stop(void)
{
	sda = 0;
	delay();
	scl = 1;
	delay();
	sda = 1;
	delay();
}
/**i2c写一字节数据**/
void i2c_write(unsigned char dat)
{
	unsigned char t = 8;
	do{
		sda = (bit)(dat & 0x80);
		dat <<= 1;
		scl = 1;
		delay();
		scl = 0;
		delay();
	}while(--t != 0);
}
/**i2c读一字节数据**/
unsigned char i2c_read(void)
{
	unsigned char dat;
	unsigned char t = 8;
	sda = 1;
	do{
		scl = 1;
		delay();
		dat <<= 1;
		if(sda){
			dat |= 0x01;
		}
		scl = 0;
		delay();
	}while(--t != 0);
	return dat;
}
/**i2c读取从机应答位**/
bit i2c_getack(void)
{
	bit ack;
	sda = 1;
	delay();
	scl = 1;
	delay();
	ack = sda;
	scl = 0;
	delay();
	return ack;
}
/**i2c主机产生应答位**/
void i2c_putack(bit ack)
{
	sda = ack;
	delay();
	scl = 1;
	delay();
	scl = 0;
	delay();
}
/**i2c发送多个字节的数据
功能：主机通过I2C总线向从机发送多个字节的数据
参数：
	SlaveAddr：从机地址（高7位是从机地址，最低位是写标志0）
	SubAddr：从机的子地址
	*dat：要发送的数据
返回：
	0：发送成功
	1：在发送过程中出现异常
**/
bit i2c_send(unsigned char slaveaddr,
unsigned char subaddr,
unsigned char Size,
char *dat)
{
	i2c_init();
	delay();
	delay();
	slaveaddr &= 0xfe;
	i2c_start();
	i2c_write(slaveaddr);
	if(i2c_getack()){
		i2c_stop();
		return 1;
	}
	i2c_write(subaddr);
	if(i2c_getack()){
		i2c_stop();
		return 1;
	}
	do{
		i2c_write(*dat++);
		if(i2c_getack()){
			i2c_stop();
			return 1;
		}
	}while(--Size != 0);
	i2c_stop();
	return 0;
}
/**i2c接收多个字节的数据
功能：主机通过I2C总线从从机接收多个字节的数据
参数：
	SlaveAddr：从机地址（高7位是从机地址，最低位是读标志1）
	SubAddr：从机的子地址
	Size：数据的字节数
	*dat：保存接收到的数据
返回：
	0：接收成功
	1：在接收过程中出现异常
**/
bit i2c_gets(unsigned char slaveaddr,
unsigned char subaddr,
unsigned char Size,
char *dat)
{
	i2c_init();
	delay();
	delay();
	slaveaddr &= 0xfe;
	i2c_start();
	i2c_write(slaveaddr);
	if(i2c_getack()){
		i2c_stop();
		return 1;
	}
	i2c_write(subaddr);
	if(i2c_getack()){
		i2c_stop();
		return 1;
	}
	i2c_start();
	slaveaddr |= 0x01;
	i2c_write(slaveaddr);
	if(i2c_getack()){
		i2c_stop();
		return 1;
	}
	for(;;){
		*dat++ = i2c_read();
		if(--Size == 0){
			i2c_putack(1);
			break;
		}
		i2c_putack(0);
	}
	i2c_stop();
	return 0;
}
#endif
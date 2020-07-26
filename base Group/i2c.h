#ifndef __I2C_H
#define __I2C_H

void delay(void);
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(unsigned char dat);
unsigned char i2c_read(void);
bit i2c_getack(void);
void i2c_putack(bit ack);
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
char *dat);
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
char *dat);


#endif
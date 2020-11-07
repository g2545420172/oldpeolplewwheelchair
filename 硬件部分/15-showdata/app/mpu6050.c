//****************************************
//MPU6050的核心驱动，
//该程序负责获取MPU6050的数据
//****************************************
#include <REG51.H>	
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library	
#include <INTRINS.H>
#include "mpu6050.h"
#include "send.h"
#include "oled.h" 
//****************************************
// 定义单片机的IIC串口，自己可以修改
//****************************************
sbit    SCL=P2^6;			//IIC时钟引脚定义
sbit    SDA=P2^7;			//IIC数据引脚定义
//**************************************************************************************************
//定义类型及变量
//**************************************************************************************************
int	dis_data;					//变量
uchar dis[6];					//显示数字(-511至512)的字符数组
//********************************************************************************
//整数转字符串
//********************************************************************************
void lcd_printf(uchar *s,int temp_data)
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}  else {
		*s=' ';
	}
	

	*++s =temp_data/10000+0x30;
	temp_data=temp_data%10000;     //取余运算

	*++s =temp_data/1000+0x30;
	temp_data=temp_data%1000;     //取余运算

	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //取余运算
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //取余运算
	*++s =temp_data+0x30;
}
//*************************************************************************************************
//************************************延时*********************************************************
//*************************************************************************************************
void delay(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<121;j++);
	}						
}
//************************************************************************************************
//延时5微秒(STC90C52RC@12M)
//不同的工作环境,需要调整此函数
//注意当改用1T的MCU时,请调整此延时函数
//************************************************************************************************
void Delay5us()
{
	unsigned char i;
	_nop_();
	i = 11;
	while (--i);
}
//*************************************************************************************************
//I2C起始信号
//*************************************************************************************************
void I2C_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}
//*************************************************************************************************
//I2C停止信号
//*************************************************************************************************
void I2C_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}
//**************************************************************************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************************************************************************
void I2C_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}
//****************************************************************************************************
//I2C接收应答信号
//****************************************************************************************************
bit I2C_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
    return CY;
}
//*****************************************************************************************************
//向I2C总线发送一个字节数据
//*****************************************************************************************************
void I2C_SendByte(uchar dat)
{
    uchar i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    I2C_RecvACK();
}
//*****************************************************************************************************
//从I2C总线接收一个字节数据
//******************************************************************************************************
uchar I2C_RecvByte()
{
    uchar i;
    uchar dat = 0;
    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}
//*****************************************************************************************************
//向I2C设备写入一个字节数据
//*****************************************************************************************************
void Single_WriteI2C(uchar REG_Address,uchar REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
//*******************************************************************************************************
//从I2C设备读取一个字节数据
//*******************************************************************************************************
uchar Single_ReadI2C(uchar REG_Address)
{
	uchar REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}
//******************************************************************************************************
//初始化MPU6050
//******************************************************************************************************
void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//******************************************************************************************************
//合成数据
//******************************************************************************************************
int GetData(uchar REG_Address)
{
	uchar H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return ((H<<8)+L);   //合成数据
}

// 串口显示加速度传感器的数据
void display(int value){
	int i;
	lcd_printf(dis,value);
	for(i=0;i<6;i++)
	{
       SendData(dis[i]);
    }
}

//对转换到的5位字符进行处理，方便OLED 屏幕显示
void OLED_process(uchar *s){
	s[0] = s[3];
	s[1] = s[4];
	s[2] = s[5];
	// 要想字符串正确显示，那么最后一位必须以0来结尾，注意不要填/0，没用
	s[3] = 0;
}


// 开始获取陀螺仪的数据
void startGetPosition(){
	unsigned char x[6],y[6],z[6];
	//串口发送数据(这里我们通过前缀来进行划分)
	SendString("$MPU6050,");
	display(GetData(ACCEL_XOUT_H));		//显示X轴加速度
	SendString(",");
	display(GetData(ACCEL_YOUT_H));		//显示Y轴加速度
	SendString(",");
	display(GetData(ACCEL_ZOUT_H));		//显示Z轴加速度
	SendString(",");
	display(GetData(GYRO_XOUT_H));		//显示X轴角速度
	SendString(",");
	display(GetData(GYRO_YOUT_H));		//显示Y轴角速度
	SendString(",");
	display(GetData(GYRO_ZOUT_H));		//显示Z轴角速度
	SendString(",0e");
	//显示方向信息
	lcd_printf(x,GetData(ACCEL_XOUT_H));
	lcd_printf(y,GetData(ACCEL_YOUT_H));
	lcd_printf(z,GetData(ACCEL_ZOUT_H));
	OLED_process(x);
	OLED_process(y);
	OLED_process(z);
	showPosition(x,y,z);
}
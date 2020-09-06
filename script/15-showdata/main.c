//****************************************
// ������
//****************************************
#include "reg51.h"
#include "send.h"
#include "mpu6050.h"
uchar dis[6];					//��ʾ����(-511��512)���ַ�����


// ������ʾ���ٶȴ�����������
void display(int value){
	int i;
	lcd_printf(dis,value);
	for(i=0;i<6;i++)
	{
       SendData(dis[i]);
    }
}

//*******************************************************************************************************
//������
//*******************************************************************************************************
void main()
{ 
	//��ʼ������
	uInit();
	delay(150);
	// ��ʼ��MPU6050
	InitMPU6050();
	delay(150);
	while(1)
	{
		display(GetData(ACCEL_XOUT_H));	//��ʾX����ٶ�
		display(GetData(ACCEL_YOUT_H));	//��ʾY����ٶ�
		display(GetData(ACCEL_ZOUT_H));	//��ʾZ����ٶ�
		display(GetData(GYRO_XOUT_H));		//��ʾX����ٶ�
		display(GetData(GYRO_YOUT_H));		//��ʾY����ٶ�
		display(GetData(GYRO_ZOUT_H));		//��ʾZ����ٶ�
		SendString("\r\n");
		delay(2000);
	}
}


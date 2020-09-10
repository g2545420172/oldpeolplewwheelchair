/************************************************************************************
* //OLED��ʾ����������Ҫ��ʾĬ�ϵ�����
*
* 1. void delay(unsigned int z) -- �����������ڵ�����ʾЧ������ʱ����,STC89C52 12MHZ z=1ʱ��Լ��ʱ1ms,����Ƶ����Ҫ�Լ�����
* 2. void main(void) -- ������
*
* History: none;
*
*************************************************************************************/

//#include "reg52.h"
#include "code-data.h"
#include "oled-iic.h"
#include "oled.h"

// OLED ��ʾ����ʼ������ʾĬ�ϵ���Ϣ��
void oledInit(void)
{

	OLED_Init(); //OLED��ʼ��
//		OLED_Fill(0xff); //��ȫ��
//		OLED_Fill(0x00); //��ȫ��
	// ��ʾ��Ϣ
	OLED_P16x16Ch(0,0,17);
	OLED_P16x16Ch(18,0,0); //��
	OLED_P16x16Ch(35,0,1); //��
	OLED_P16x16Ch(52,0,2); //��
	OLED_P16x16Ch(69,0,3); //��
	OLED_P16x16Ch(86,0,4); //��
	OLED_P16x16Ch(103,0,5); //��
	//��ʾ����
	OLED_P16x16Ch(0,2,18);
	OLED_P16x16Ch(18,2,6); //��
	OLED_P16x16Ch(35,2,7); //��
	OLED_P8x16Str(52,2,":000");
	//��ʾѪѹ����
	OLED_P16x16Ch(0,4,19);
	OLED_P16x16Ch(18,4,8); //Ѫ
	OLED_P16x16Ch(35,4,9); //ѹ
	OLED_P8x16Str(52,4,"H000 L000");
	//��ʾ������
	OLED_P16x16Ch(0,6,20);
	OLED_P8x16Str(18,6,"000,000,000");
//		OLED_CLS();//����
//		Draw_BMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
//		Draw_BMP(0,0,128,8,BMP2);
	
}

// ��ʾ����
void showHeart(unsigned char str[]){
	OLED_P8x16Str(61,2,str);
}

//��ʾѪѹ
void showPress(unsigned char h[],unsigned char l[]){
	OLED_P8x16Str(61,4,h);
	OLED_P8x16Str(101,4,l);
}

////��ȡ������ĳ���ַ�
//char* showChar(unsigned char arr[],int positin){
//	unsigned char ch[2];
//	ch[0] = "6";
//	ch[1] = "\0";
//	return ch;
//}

//��ʾ����������
void showPosition(unsigned char x[],unsigned char y[],unsigned char z[]){
	OLED_P8x16Str(18,6,x);
	// ע�ⵥ���ź�˫���ŵ�����
	OLED_P8x16Str(42,6,",");
	OLED_P8x16Str(50,6,y);
	OLED_P8x16Str(74,6,",");
	OLED_P8x16Str(82,6,z);
}

/***
����ģ������������������ǲ��õ��Ǵ���2
��ģ����Ҫ�����ȡ�ʹ���������Ϣ
***/
#ifndef __HEART_H
#define __HEART_H
/*���ڿ��ƺ����ĺ���˵��*/

void u2Init();
void SendData2(unsigned char dat);
void SendString2(char *s);
void getHeart();
void Startheart();
char * hex2char(char * str,unsigned char dat);
#endif
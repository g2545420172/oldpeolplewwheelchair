#ifndef __OLED_H
#define __OLED_H
/*���ڿ��ƺ����ĺ���˵��*/
void oledInit();
void showHeart(char *str);
void showPress(unsigned char h[],unsigned char l[]);
void showPosition(unsigned char x[],unsigned char y[],unsigned char z[]);
#endif
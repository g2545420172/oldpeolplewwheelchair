#ifndef __OLED_H
#define __OLED_H
/*���ڿ��ƺ����ĺ���˵��*/
void oledInit();
void showHeart(char *str);
void showPressH(unsigned char h[]);
void showPressL(unsigned char l[]);
void showPosition(unsigned char x[],unsigned char y[],unsigned char z[]);
#endif
#include "Arduino.h"

void lcdUpdateScreen();
void Setup_Run_LCD_Intro();
void Print_LCD_Wire();
void Print_LCD_Mowing();
void Print_LCD_Compass_Mowing();
void Print_LCD_Info_Manuel();
void Print_LCD_NO_Wire();
void Print_LCD_Wire_ON();
void Print_Sonar_Hit();
void Print_LCD_Compass_Home();
void Print_LCD_Heading_for_Home();
void Print_LCD_Menu_Timing(byte LCD_Menu_Timing);
void printLoop(byte lCount);
void printFindFire();
void printMessage(char* msg);

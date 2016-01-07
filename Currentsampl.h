

#ifndef CURRENTSAMPLING_H 
#define CURRENTSAMPLING_H 

#include "Type.h"

#define CHARGING_1 6
#define CHARGING_2 4

#define ADD_HAT1 0
#define ADD_HAT2 8
#define ADD_HAT3 1
#define ADD_HAT4 5
#define ADD_HAT5 2

#define IN_VOLTAGE 3
#define BAT_VOLTAGE 9
#define TEMP 7

void CurrentsamplInit(void);
u16 CurrentsamplGetAd(u8 passage);
float CurrentsamplGetCurrent(u8 passage);
float CurrentsamplGetVoltage(u8 passage);
void CurrentSetEn(u8 cmd1, u8 cmd2);
void CurrentsamplCheckAsk(u8 num, float current) ;

#endif 

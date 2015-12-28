

#ifndef CURRENTSAMPLING_H 
#define CURRENTSAMPLING_H 

#include "Type.h"

#define CHARGING_1 5
#define CHARGING_2 6

#define ADD_HAT1 1
#define ADD_HAT2 8
#define ADD_HAT3 2
#define ADD_HAT4 4
#define ADD_HAT5 0

#define IN_VOLTAGE 3
#define BAT_VOLTAGE 9
#define TEMP 7

void CurrentsamplInit(void);
u16 CurrentsamplGetAd(u8 passage);
float CurrentsamplGetCurrent(u8 passage);
float CurrentsamplGetVoltage(u8 passage);

#endif 

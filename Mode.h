

#ifndef MODE_H
#define MODE_H

#include "Type.h"

void ModeInit(void);

void ModeShowBreath(void);

void ModeSetRing(u8 cmd,u8 data);

void ModeSetRotation(u8 cmd);

void ModeSetColour(u8 cmd, u8 r, u8 g, u8 b);

void ModeSetDevice(u8 data1, u8 data2, u8 data3, u8 data4);
void ModeSetLed(u8 data1, u8 data2, u8 data3, u8 data4);
void ModeSetPhoto(u8 data1, u8 data2, u8 data3, u8 data4);

u8 ModeGetPwm(u8 num);

void ModeAllShutdown(void);

#endif

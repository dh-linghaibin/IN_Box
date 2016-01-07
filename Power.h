

#ifndef POWER_H
#define POWER_H

#include "Type.h"


void PowerAdd(u8 num, float power);
float PowerGet(u8 num);
float PowerGetAll(void);
void PowerSub(u8 num, float power);
void PowerBankSet(u8 num, u16 power);
void PowerSetBit(u8 bit, u8 cmd);
void PowerInit(void);
void PowerLockingSet(u8 num, u8 percentage);
u8 PowerLockingCloose(u8 num);

#endif


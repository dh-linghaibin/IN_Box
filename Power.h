

#ifndef POWER_H
#define POWER_H

#include "Type.h"


void PowerAdd(u8 num, float power);
float PowerGet(u8 num);
float PowerGetAll(void);
void PowerSub(u8 num, float power);
void PowerBankSet(u8 num, u16 power);

#endif


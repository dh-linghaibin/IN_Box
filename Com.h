
#ifndef COM_H
#define COM_H

#include "Type.h"

void ComInit(void);

u8 ComGetRingData(u8 num);
u8 ComGetRingFlag(void);
void ComClearRingFlag(void);

u8 ComGetWatchData(u8 num);
u8 ComGetWatchFlag(void);
void ComClearWatchFlag(void);

void ComSendCmdWatch(u8 cmd,u8 par1,u8 par2,u8 par3,u8 par4);

#endif


#ifndef BLUETOOTH_H 
#define BLUETOOTH_H

#include "Type.h"

void BluetoothInit(void);
void BluetoothSendSrt(u8 * pd);
void BluetoothSend(u8 cmd,
               u8 date0,u8 data1,
               u8 data2,u8 data3,
               u8 data4,u8 data5,
               u8 data6,u8 data7);
u8 BluetoothGetFlag(void);
void BluetoothClearFlag(void);
u8 BluetoothGetDataRx(u8 num);
u8 BluetoothCheck(void);

void BluetoothTimeServive(void);

void BluetoothSetEn(u8 cmd);

#endif



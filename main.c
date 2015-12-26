/*!
 @header     文件名
 @abstract   简介，概念
 @discussion 用途，方法
 @copyright  版权
 */
#include "Sys.h"
#include "Show.h"
#include "Usbout.h"
#include "Currentsampl.h"
#include "Bluetooth.h"
#include "Com.h"
#include "Eeprom.h"
#include "Timer.h"
#include "Mode.h"
     
int main(void) {
    SysInit();
    EeepromInit();
    UsboutInit();
    ShowInit();
    CurrentsamplInit();
    BluetoothInit();
    ComInit();
    TimerInit();
    ModeInit();
    INTEN
    while(1) {
        ModeShowBreath();
        if(TimerGetSecFlag() == 0x80) {//One second arrive
            TimerClearSecFlag();//Clear flag
            
        }
        if(ComGetRingFlag() == 0x80) {
            ComClearRingFlag();
            switch(ComGetRingData(0)) {
                case 0x22:
                    ModeSetRing(ComGetRingData(1),ComGetRingData(2));
                break;
                case 0x23:
                    ModeSetRotation(ComGetRingData(1));
                break;
                case 0x44://sleep
                break;
                default:break;
            }
        }
        if(BluetoothGetFlag() == 0x80) {
            BluetoothClearFlag();
            if(BluetoothCheck() == 0x80) {
                
            } else {
                BluetoothSend(0x44,0,0,0,0,0,0,0,0);
            }
        }
        
    }
}




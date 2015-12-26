/*!
 @header     �ļ���
 @abstract   ��飬����
 @discussion ��;������
 @copyright  ��Ȩ
 */
#include "Sys.h"
#include "Show.h"
#include "Usbout.h"
#include "Currentsampl.h"
#include "Bluetooth.h"
#include "Com.h"
#include "Eeprom.h"
#include "Timer.h"
     
int main(void) {
    SysInit();
    EeepromInit();
    UsboutInit();
    ShowInit();
    CurrentsamplInit();
    BluetoothInit();
    ComInit();
    TimerInit();
    INTEN
    while(1) {
        if(TimerGetSecFlag() == 0x80) {//One second arrive
            TimerClearSecFlag();//Clear flag
            
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




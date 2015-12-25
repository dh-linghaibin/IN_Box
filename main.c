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

int main( void )
{
    SysInit();
    UsboutInit();
    ShowInit();
    CurrentsamplInit();
    BluetoothInit();
    while(1) {
        
    }
}

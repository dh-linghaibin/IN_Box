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

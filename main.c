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
#include "Prompt.h"
#include "Power.h"
     
int main(void) {
    SysInit();
    EeepromInit();
    UsboutInit();
    PromptInit();
    ShowInit();
    CurrentsamplInit();
    BluetoothInit();
    ComInit();
    TimerInit();
    ModeInit();
    INTEN
    while(1) {
        ModeShowBreath();
        PromptTimeServive();
        BluetoothTimeServive();
        if(TimerGetSecFlag() == 0x80) {//One second arrive
            float only_once_current = 0;
            TimerClearSecFlag();//Clear flag
            if(ModeGetPwm(0) > 0x00) {
                only_once_current = CurrentsamplGetCurrent(ADD_HAT1);
                PowerAdd(0,only_once_current);
                BluetoothSend(0x70,(u8)(only_once_current*100),0,0,0,0,0,0,0);
            }
            if(ModeGetPwm(1) > 0x00) {
                only_once_current = CurrentsamplGetCurrent(ADD_HAT2);
                PowerAdd(1,only_once_current);
                BluetoothSend(0x70,0,(u8)(only_once_current*100),0,0,0,0,0,0);
            }   
            if(ModeGetPwm(2) > 0x00) {
                only_once_current = CurrentsamplGetCurrent(ADD_HAT3);
                PowerAdd(2,only_once_current);
                BluetoothSend(0x70,0,0,(u8)(only_once_current*100),0,0,0,0,0);
            }
            if(ModeGetPwm(3) > 0x00) {
                only_once_current = CurrentsamplGetCurrent(ADD_HAT4);
                PowerAdd(3,only_once_current);
                BluetoothSend(0x70,0,0,0,(u8)(only_once_current*100),0,0,0,0);
            }
            if(ModeGetPwm(4) > 0x00) {
                only_once_current = CurrentsamplGetCurrent(ADD_HAT5);
                PowerAdd(4,only_once_current);
                BluetoothSend(0x70,0,0,0,0,(u8)(only_once_current*100),0,0,0);
            }
            
            if(CurrentsamplGetVoltage(IN_VOLTAGE) > 4.0) {
                PowerSub(0,CurrentsamplGetCurrent(CHARGING_1));
                PowerSub(1,CurrentsamplGetCurrent(CHARGING_2));
            }
            //温度
            
            //口袋
            PromptPocketReflect();
        }
        if(ComGetRingFlag() == 0x80) {
            ComClearRingFlag();
            PromptMucicReflect();
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
                u16 intermediate = 0;
                u8 read_i = 0;
                u8 x1 = 0;
                u8 x2 = 0;
                switch(BluetoothGetDataRx(1)) {
                    case 0x01://2路手机充电电流值
                        x1 = (u16)CurrentsamplGetCurrent(CHARGING_1);
                        x2 = (u16)CurrentsamplGetCurrent(CHARGING_2);
                        BluetoothSend(0x01,x1>>8,x1,x2>>8,x2,0,0,0,0);
                    break;
                    case 0x03://温度检测
                        intermediate = CurrentsamplGetAd(TEMP);
                        BluetoothSend(0x03,intermediate,intermediate>>8,0,0,0,0,0,0);
                    break;
                    case 0x04://7个5050LED数码管显示
                        ModeSetColour(BluetoothGetDataRx(2),
                                      BluetoothGetDataRx(3),
                                      BluetoothGetDataRx(4),
                                      BluetoothGetDataRx(5));
                        BluetoothSend(0x04,BluetoothGetDataRx(2),
                                           BluetoothGetDataRx(3),
                                           BluetoothGetDataRx(4),
                                           BluetoothGetDataRx(5),0,0,0,0);
                    break;
                    case 0x05://设置设备按钮功能
                        ModeSetDevice(BluetoothGetDataRx(2),
                                      BluetoothGetDataRx(3),
                                      BluetoothGetDataRx(4),
                                      BluetoothGetDataRx(5));
                        BluetoothSend(0x05,BluetoothGetDataRx(2),
                                           BluetoothGetDataRx(3),
                                           BluetoothGetDataRx(4),
                                           BluetoothGetDataRx(5),0,0,0,0);
                    break;
                    case 0x06://霍尔检测
                        if(PromptPocket() == 0x80) {
                            BluetoothSend(0x06,0x01,0,0,0,0,0,0,0);
                        } else {
                            BluetoothSend(0x06,0x00,0,0,0,0,0,0,0);
                        }
                    break;
                    case 0x07://修改拍照模式下按钮功能
                        ModeSetPhoto(BluetoothGetDataRx(2),
                                      BluetoothGetDataRx(3),
                                      BluetoothGetDataRx(4),
                                      BluetoothGetDataRx(5));
                        BluetoothSend(0x07,BluetoothGetDataRx(2),
                                           BluetoothGetDataRx(3),
                                           BluetoothGetDataRx(4),
                                           BluetoothGetDataRx(5),0,0,0,0);
                    break;
                    case 0x08://修改音乐模式下按钮功能
                        BluetoothSend(0x08,BluetoothGetDataRx(2),
                                           BluetoothGetDataRx(3),
                                           BluetoothGetDataRx(4),
                                           BluetoothGetDataRx(5),0,0,0,0);
                    break;
                    case 0x09://修改LED模式下按钮功能
                        ModeSetLed(BluetoothGetDataRx(2),
                                      BluetoothGetDataRx(3),
                                      BluetoothGetDataRx(4),
                                      BluetoothGetDataRx(5));
                        BluetoothSend(0x09,BluetoothGetDataRx(2),
                                           BluetoothGetDataRx(3),
                                           BluetoothGetDataRx(4),
                                           BluetoothGetDataRx(5),0,0,0,0);
                    break;
                    case 0x0a://自定义移动电源电量
                        x1 = BluetoothGetDataRx(3);
                        x1 |= BluetoothGetDataRx(4) << 8;
                        PowerBankSet(BluetoothGetDataRx(2),x1);
                    break;
                    case 0x12://移动电源状态
                        if(CurrentsamplGetVoltage(IN_VOLTAGE) > 4.0) {
                            if(CurrentsamplGetCurrent(CHARGING_1) > 10) {
                                x1 = 1;
                            }
                            if(CurrentsamplGetCurrent(CHARGING_2) > 10) {
                                x2 = 1;
                            }
                        }
                        intermediate = (u16)PowerGetAll();
                        BluetoothSend(0x12,(u8)(intermediate),
                                           (u8)(intermediate<<8),
                                           x1,x2,0,0,0,0);
                    break;
                    case 0x13://HUB内置电量
                        BluetoothSend(0x13,100,100,0,0,0,0,0,0);
                    break;
                    case 0x0e://端口输出电量
						BluetoothSend(0x0e,(u8)PowerGet(0),(u8)PowerGet(1),(u8)PowerGet(2)
                                    ,(u8)PowerGet(3),(u8)PowerGet(4),0,0,0);
					break;
                    case 0x0f://端口锁定电量
							//先不做
					break;
                    case 0x0c://端口充电时间
                        BluetoothSend(0x0c,100,100,0,0,0,0,0,0);
                    break;
                    case 0x0d://温度设置
                        
                    break;
                    case 0x10://移动电源设置 打开还是关闭
                    if(BluetoothGetDataRx(2) == 0x00) {
                        
                    } else {

                    }
                    BluetoothSend(0x10,BluetoothGetDataRx(2),
                                       BluetoothGetDataRx(3),
                                       0,0,0,0,0,0);
                    break;
                    break;
                    case 0x14://磁感安全设置
                        PromptPocketSetFlag(BluetoothGetDataRx(2));
                        BluetoothSend(0x14,BluetoothGetDataRx(2),
                                            0,0,0,0,0,0,0);
                    break;
                    case 0x11://设备设置与状态
                        for(read_i = 2; read_i < 6; read_i++) {
                            if(BluetoothGetDataRx(read_i) == 1) {
                                UsboutSet(read_i-2, 0xff);
                            } else {
                                UsboutSet(read_i-2, 0x00);
                            }
                        }   
                    break;  
                    case 0x02://触摸按键反馈
                        PromptMucicSetFlag(BluetoothGetDataRx(2));
                        BluetoothSend(0x02,BluetoothGetDataRx(2),
                                            0,0,0,0,0,0,0);
					break;
                    case 0x70://即使电流量采集
                        BluetoothSend(0x70,(u8)(CurrentsamplGetCurrent(ADD_HAT1)*100),
                                           (u8)(CurrentsamplGetCurrent(ADD_HAT2)*100),
                                           (u8)(CurrentsamplGetCurrent(ADD_HAT3)*100),
                                           (u8)(CurrentsamplGetCurrent(ADD_HAT4)*100),
                                           (u8)(CurrentsamplGetCurrent(ADD_HAT5)*100),
                                           0,0,0);
					break;
                    default:break;
                }
            } else {
                BluetoothSend(0x44,0,0,0,0,0,0,0,0);
            }
        }
        
    }
}




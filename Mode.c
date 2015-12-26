
#include "Mode.h"
#include "Eeprom.h"
#include "Usbout.h"
#include "Show.h"
#include "Bluetooth.h"

static u8 mode_mode_flag = 0;
static u8 mode_devic_flag[4] = {0,0,0,0};
static u8 mode_led_flag[4] = {0,0,0,0};
static u8 mode_photo_flag[4] = {0,0,0,0};

static u8 mode_out_pwm[5] = {0,0,0,0,0};
static u8 mode_led_rgb[24] = {
    0x00,0xff,0x00,
    0xff,0x00,0x00,
    0x04,0x00,0xff,
    0x55,0xf5,0x23,
    0x55,0x55,0x00,
    0x00,0x55,0x55,
    0x55,0x00,0x55,
    0x55,0x00,0x55,
};
static u8 mode_reg_flag = 0;
static u8 mode_rgb_mode = 0;

void ModeInit(void) {
    u8 read_i = 0;
    if(EepromRead(10) == 0x55) {
        mode_mode_flag = EepromRead(11);
        for(read_i = 0;read_i < 4;read_i++) {
            mode_devic_flag[read_i] = EepromRead(12+read_i);
        }
        for(read_i = 0;read_i < 4;read_i++) {
            mode_led_flag[read_i] = EepromRead(16+read_i);
        }
        for(read_i = 0;read_i < 4;read_i++) {
            mode_photo_flag[read_i] = EepromRead(20+read_i);
        }
        for(read_i = 0;read_i < 5;read_i++) {
            mode_out_pwm[read_i] = EepromRead(25+read_i);
        }
        for(read_i = 0;read_i < 24;read_i++) {
            mode_led_rgb[read_i] = EepromRead(30+read_i);
        }
        EepromWrite(54,0);
        mode_rgb_mode = EepromRead(54);
        mode_reg_flag = EepromRead(55);
        for(read_i = 0;read_i < 5;read_i++) {
            UsboutSet(read_i,mode_out_pwm[read_i]);
        }
    } else {
        EepromWrite(10,0x55);
        for(read_i = 0;read_i < 5;read_i++) {
            UsboutSet(read_i,0x00);
        }
        for(read_i = 0;read_i < 24;read_i++) {
            EepromWrite(30+read_i,mode_led_rgb[read_i]);
        }
    }
}

static u8 ModeCheck(u8 cmd) {
    u8 mode_data = 0;
    switch(cmd) {
        case 0:
            mode_data = mode_devic_flag[cmd];
        break;
        case 1:
            mode_data = mode_led_flag[cmd];
        break;
        case 2:
            mode_data = mode_photo_flag[cmd];
        break;
    }
    return mode_data;
}

static void ModeSet(u8 mode, u8 data) {
    switch(mode) {
        case 0:
            if(mode_out_pwm[data] > 0) {
                mode_out_pwm[data] = 0x00;
            } else {
                mode_out_pwm[data] = 0xff;
            }
            EepromWrite(25+data,mode_out_pwm[data]);
            UsboutSet(data,mode_out_pwm[data]);
        break;
        case 1:
            mode_rgb_mode = data;
            EepromWrite(55,mode_rgb_mode);
        break;
        case 2:
            switch(data) {
                case 1:
                    BluetoothSend(0x50,0,0,0,0,0,0,0,0);
                break;
                case 2:
                    BluetoothSend(0x51,0,0,0,0,0,0,0,0);
                break;
                case 3:
                    BluetoothSend(0x52,0,0,0,0,0,0,0,0);
                break;
                case 4:
                    BluetoothSend(0x53,0,0,0,0,0,0,0,0);
                break;
            }
        break;
        default:break;
    }
}

void ModeShowBreath(void) {
    static u16 count = 0;
    static u8 val = 0;
    static u8 run_Falg = 0;
    static u8 Duty_Val_1 = 0;
    static u8 Duty_Val_2 = 0;
    static u8 Duty_Val_3 = 0;
    count++;
    if(mode_rgb_mode == 0) {
        if(run_Falg == 0) {
            if(val < 0xfe){
                if(count >= 600) {
                    count = 0;
                    if( (Duty_Val_1 < mode_led_rgb[mode_reg_flag*3]) || 
                        (Duty_Val_2 < mode_led_rgb[mode_reg_flag*3+1]) ||
                        (Duty_Val_3 < mode_led_rgb[mode_reg_flag*3+2])
                        ) 
                    {
                        if( Duty_Val_1 < mode_led_rgb[mode_reg_flag*3] )
                            Duty_Val_1++;
                        if( Duty_Val_2 < mode_led_rgb[mode_reg_flag*3+1] )
                            Duty_Val_2++;
                        if( Duty_Val_3 < mode_led_rgb[mode_reg_flag*3+2] )
                            Duty_Val_3++;
                        ShowRgbSed(Duty_Val_1,Duty_Val_2,Duty_Val_3);
                    }
                    val++;
                }
            } else {
                run_Falg = 1;
                count = 0;
            }
        } else if(run_Falg == 1) {
            if(count >= 8000) {
                count = 0;
                run_Falg = 2;
            }
        } else if(run_Falg == 2) {
            if(val > 0x00){
                if(count >= 600) {
                    count = 0;
                    val--;
                    if( (Duty_Val_1 > 0) || (Duty_Val_2 > 0) || (Duty_Val_3 > 0)) {
                        if(Duty_Val_1 > 0)
                            Duty_Val_1--;
                        if(Duty_Val_2 > 0)
                            Duty_Val_2--;
                        if(Duty_Val_3 > 0)
                            Duty_Val_3--;
                       ShowRgbSed(Duty_Val_1,Duty_Val_2,Duty_Val_3);
                    }
                }
            }else {
                run_Falg = 3;
                count = 0;
            } 
        } else if(run_Falg == 3) {
            if(count >= 15000) {
                run_Falg = 0;
            }
        }
    } else if(mode_rgb_mode == 1) {   
        
    } else if(mode_rgb_mode == 2) {
        
    } else {
    }
}

void ModeSetRing(u8 cmd,u8 data) {
    if((cmd&0x10) == 0x10) { //mode
        mode_mode_flag = data;
        EepromWrite(11,mode_mode_flag);
    } else {
        ModeSet(mode_mode_flag,ModeCheck(mode_mode_flag));
    }
}

void ModeSetRotation(u8 cmd) {
    if(cmd == 0xff) {
        if(mode_mode_flag == 0) {
            
        } else if(mode_mode_flag == 1) {
            if(mode_reg_flag < 8) {
                mode_reg_flag++;
            } else {
                mode_reg_flag = 0;
            }
            EepromWrite(56,mode_reg_flag);
        }
    } else if(cmd == 0x55) {
        if(mode_mode_flag == 0) {
            
        } else if(mode_mode_flag == 1) {
            if(mode_reg_flag > 0x00) {
                mode_reg_flag--;
            } else {
                mode_reg_flag = 8;
            }
            EepromWrite(56,mode_reg_flag);
        }
    }
}
        
            
            

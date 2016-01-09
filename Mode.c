
#include "Mode.h"
#include "Eeprom.h"
#include "Usbout.h"
#include "Show.h"
#include "Bluetooth.h"
#include "Com.h"
#include "Delay.h"
#include "Prompt.h"
#include "Currentsampl.h"

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
static u8 out_bit = 0x00;

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
        mode_rgb_mode = EepromRead(54);
        mode_reg_flag = EepromRead(55);
        for(read_i = 0;read_i < 5;read_i++) {
            UsboutSet(read_i,mode_out_pwm[read_i]);
            if(mode_out_pwm[read_i] > 0x00) {
                out_bit &= ~BIT(read_i);
            } else {
                out_bit |= BIT(read_i);
            }
            //mode_out_pwm[read_i] = 0xff;
            //UsboutSet(read_i,0xff); //Test Use
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

static u8 ModeCheck(u8 cmd,u8 data) {
    u8 mode_data = 0;
    u8 bit = 0;
    switch (data) {
        case 0x01:
            bit = 0;
        break;
        case 0x02:
            bit = 1;
        break;
        case 0x04:
            bit = 2;
        break;
        case 0x08:
            bit = 3;
        break;
    }
    switch(cmd) {
        case 0:
            mode_data = mode_devic_flag[bit];
        break;
        case 1:
            mode_data = mode_led_flag[bit];
        break;
        case 2:
            mode_data = mode_photo_flag[bit];
        break;
    }
    return mode_data;
}

static void ModeSet(u8 mode, u8 data) {
    switch(mode) {
        case 0:
            if(mode_out_pwm[data] > 0) {
                mode_out_pwm[data] = 0x00;
                out_bit &= ~BIT(data);
            } else {
                mode_out_pwm[data] = 0xff;
                out_bit |= BIT(data);
            }
            EepromWrite(25+data,mode_out_pwm[data]);
            UsboutSet(data,mode_out_pwm[data]);
            ComSendCmdWatch(0x05,out_bit,0,0,0);
        break;
        case 1:
            if(data == 4) {
                if(mode_rgb_mode == 3) {
                    mode_rgb_mode = 4;
                } else {
                    mode_rgb_mode = 3;
                }
            } else {
                mode_rgb_mode = data-1;
            }
            EepromWrite(54,mode_rgb_mode);
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
        if(count > 65530) {
            count  = 0;
            if(val > 0) {
                val = 0x00;
                ShowRgbSed(mode_led_rgb[mode_reg_flag*3],
                           mode_led_rgb[mode_reg_flag*3+1],
                           mode_led_rgb[mode_reg_flag*3+2]);
            } else {
                val = 0xff;
                ShowRgbSed(0x00,0x00,0x00);
            }
        }
    } else if(mode_rgb_mode == 2) {
         if(count > 30000) {
            count  = 0;
            if(val > 0) {
                val = 0x00;
                ShowRgbSed(mode_led_rgb[mode_reg_flag*3],
                           mode_led_rgb[mode_reg_flag*3+1],
                           mode_led_rgb[mode_reg_flag*3+2]);
            } else {
                val = 0xff;
                ShowRgbSed(0x00,0x00,0x00);
            }
        }
    } else if(mode_rgb_mode == 3) {
       if(count > 10000) {
           count = 0;
            ShowRgbSed(mode_led_rgb[mode_reg_flag*3],
                   mode_led_rgb[mode_reg_flag*3+1],
                   mode_led_rgb[mode_reg_flag*3+2]);
        }
    } else if(mode_rgb_mode == 4) {
        if(count > 10000) {
            count = 0;
            ShowRgbSed(0x00,0x00,0x00);
        }
    }
}

void ModeSetRing(u8 cmd,u8 data) {
    if((cmd&0x10) == 0x10) { //mode
        mode_mode_flag = data;
        ComSendCmdWatch(mode_mode_flag+1,
                        ModeCheck(mode_mode_flag,0x01),
                        ModeCheck(mode_mode_flag,0x02),
                        ModeCheck(mode_mode_flag,0x04),
                        ModeCheck(mode_mode_flag,0x08));
        EepromWrite(11,mode_mode_flag);
    } else {
        ModeSet(mode_mode_flag,ModeCheck(mode_mode_flag, cmd));
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
            EepromWrite(55,mode_reg_flag);
        }
    } else if(cmd == 0x55) {
        if(mode_mode_flag == 0) {
            
        } else if(mode_mode_flag == 1) {
            if(mode_reg_flag > 0x00) {
                mode_reg_flag--;
            } else {
                mode_reg_flag = 8;
            }
            EepromWrite(55,mode_reg_flag);
        }
    }
}
        
void ModeSetColour(u8 cmd, u8 r, u8 g, u8 b) {
    mode_reg_flag = cmd;
    mode_led_rgb[cmd*3] = r;
    mode_led_rgb[cmd*3+1] = g;
    mode_led_rgb[cmd*3+2] = b;
    EepromWrite(30+cmd*3,mode_led_rgb[cmd*3]);
    EepromWrite(30+cmd*3+1,mode_led_rgb[cmd*3+1]);
    EepromWrite(30+cmd*3+2,mode_led_rgb[cmd*3+2]);
    EepromWrite(55,mode_reg_flag);
    ShowRgbSed(r, g, b);
}

void ModeSetDevice(u8 data1, u8 data2, u8 data3, u8 data4) {
    u8 read_i = 0;
    mode_devic_flag[0] = data1;
    mode_devic_flag[1] = data2;
    mode_devic_flag[2] = data3;
    mode_devic_flag[3] = data4;
    for(read_i = 0;read_i < 4;read_i++) {
        EepromWrite(12+read_i,mode_devic_flag[read_i]);
    }
}

void ModeSetLed(u8 data1, u8 data2, u8 data3, u8 data4) {
    u8 read_i = 0;
    mode_led_flag[0] = data1;
    mode_led_flag[1] = data2;
    mode_led_flag[2] = data3;
    mode_led_flag[3] = data4;
    for(read_i = 0;read_i < 4;read_i++) {
        EepromWrite(16+read_i,mode_led_flag[read_i]);
    }
}

void ModeSetPhoto(u8 data1, u8 data2, u8 data3, u8 data4) {
    u8 read_i = 0;
    mode_photo_flag[0] = data1;
    mode_photo_flag[1] = data2;
    mode_photo_flag[2] = data3;
    mode_photo_flag[3] = data4;
    for(read_i = 0;read_i < 4;read_i++) {
        EepromWrite(20+read_i,mode_photo_flag[read_i]);
    }
}

u8 ModeGetPwm(u8 num) {
    return mode_out_pwm[num];
}

void ModeAllShutdown(void) {
    u8 set_i = 0;
    for(set_i = 0;set_i < 5;set_i++) {
        UsboutSet(set_i,0x00); //Test Use
    }
    ShowRgbSed(0x00,0x00,0x00);
    PromptMusicShowdown();
    BluetoothSendSrt("TTM:RENPowearIN");
    BluetoothSetEn(1);//cloose blue
    CurrentSetEn(1, 1);//cloose check
    PromptSetEn(1);//cloose
    PB_CR2_C23 = 1; //open int
    ComSendCmdWatch(0x07,0,0,0,0);
    DelayMs(800);
    MCUSLEEP
}

void ModeAllOpen(void) {
    PB_CR2_C23 = 0; //close int
    BluetoothSetEn(0);//open blue
    CurrentSetEn(0, 0);//open check
    PromptSetEn(0);//open
}

void ModeSetOut(u8 num,u8 data) {
    mode_out_pwm[num] = data;
    EepromWrite(25+num,mode_out_pwm[num]);
    UsboutSet(num,mode_out_pwm[num]);
}

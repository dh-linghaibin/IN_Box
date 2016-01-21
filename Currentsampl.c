


#include "Currentsampl.h"
#include "Delay.h"
#include "Bluetooth.h"
#include "Eeprom.h"
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
static u16 current_erroe[5] = {0,0,0,0,0};

void CurrentsamplInit(void) {
    u8 read_i = 0;
    PB_DDR = 0x00;
    PB_CR1 = 0x00;
    PB_CR2 = 0x00;
    
    PE_DDR_DDR6= 0;
    PE_CR1_C16 = 0;
    PE_CR2_C26 = 0;
    
    PE_DDR_DDR7= 0;
    PE_CR1_C17 = 0;
    PE_CR2_C27 = 0;
    
    PE_DDR_DDR2= 1;//EN
    PE_CR1_C12 = 1;
    PE_CR2_C22 = 0; //0
    
    PC_DDR_DDR7= 1;//EN
    PC_CR1_C17 = 1;
    PC_CR2_C27 = 0; //0
    
    ADC_CSR &= ~BIT(6);
	ADC_CSR &= ~BIT(4);
    ADC_CR1 |= 0x70;
	ADC_CR1 |= BIT(1);
	ADC_CR2 |= BIT(1);
	ADC_TDRL = 0x01;
    
    EXTI_CR1 |= BIT(2);//¿ªÆôPB¿ÚÖÐ¶Ï
	EXTI_CR1 &= ~BIT(3);
    
    PE_ODR_ODR2 = 0;
    PC_ODR_ODR7 = 0;
    if(EepromRead(150) != 0x55) {
        EepromWrite(150,0x55);                 
        for(read_i = 0; read_i < 4; read_i++) {
            current_erroe[read_i] = 
                CurrentsamplGetAd(CurrentFinishingchannel(read_i));
            EepromWrite(151+(read_i*2),(u8)(current_erroe[read_i]));
            EepromWrite(152+(read_i*2),(u8)(current_erroe[read_i] >> 8));
        }
    }
    for(read_i = 0; read_i < 4; read_i++) {
        current_erroe[read_i] = EepromRead(151+(read_i*2));
        current_erroe[read_i] |= (EepromRead(152+(read_i*2)) << 8);
    }
}
u8 CurrentFinishingchannel(u8 channel) {
    switch(channel) {
        case 0:
            return ADD_HAT1;
        break;
        case 1:
            return ADD_HAT2;
        break;
        case 2:
            return ADD_HAT3;
        break;
        case 3:
            return ADD_HAT4;
        break;
        case 4:
            return ADD_HAT5;
        break;
        default:break;
    }
    return 0;
}
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
void CurrentSetEn(u8 cmd1, u8 cmd2) {
    PE_ODR_ODR2 = cmd1;
    PC_ODR_ODR7 = cmd2;
}
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
u16 CurrentsamplGetAd(u8 passage) {
    u16 ad_value = 0;
    u8 ad_h = 0;
    u8 ad_l = 0;
	ADC_CSR = passage;
	ADC_CR1 = 0x01;
	DelayUs(40);
	ADC_CR1 = ADC_CR1 | 0x01; 
	while((ADC_CSR & 0x80) == 0); 
    ad_h = ADC_DRH;
    ad_l = ADC_DRL;
	ad_value = (ad_h<<8) + ad_l;
    return ad_value;
}
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
u16 xx[5] = 0;
float CurrentsamplGetCurrent(u8 passage) {
    float current = 0;
    u16 ad_value = 0;
    u16 ad_last = 0;
    u8 change = 0;
    switch(passage) {
        case ADD_HAT1:
            change = 0;
        break;
        case ADD_HAT2:
            change = 1;
        break;
        case ADD_HAT3:
            change = 2;
        break;
        case ADD_HAT4:
            change = 3;
        break;
        case ADD_HAT5:
            change = 4;
        break;
        default:break;
    }
    ad_last = CurrentsamplGetAd(passage);
    if(ad_last > current_erroe[change]) {
        ad_value = (u16)(ad_last - current_erroe[change]);//count error
        xx[change] = ad_value;
    }
    if(ad_value > 1000) {
        current = ((5.91870967741*ad_value)/65535); //(1/1.55)*3.3*2.87
    } else {
        current = 0;
    }
    if(current < 0) {
        current = 0;
    }
	return current;
}
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
float CurrentsamplGetVoltage(u8 passage) {
    float voltage = 0;
    u16 ad_value = 0;
    ad_value = CurrentsamplGetAd(passage);
    voltage = ((6.6*ad_value)/65535);
    if(voltage < 0) {
        voltage = 0;
    }
    return voltage; 
}


void CurrentsamplCheckAsk(u8 num, float current) {
    static u8 ask[5] = {0,0,0,0,0};
    if(current > 0.005) {
        ask[num] = (u8)(current*10);
    } else {
        ask[num] = 0;
    }
    BluetoothSend(0x70,ask[0],ask[1],ask[2],ask[3],ask[4],0,0,0);
}

#pragma vector=6
__interrupt void EXTI_PORTB_IRQHandler(void)
{
    INTOFF

    INTEN
}

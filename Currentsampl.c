


#include "Currentsampl.h"
#include "Delay.h"
#include "Bluetooth.h"
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
void CurrentsamplInit(void) {
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
    
    PE_ODR_ODR2 = 0;
    PC_ODR_ODR7 = 0;
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
	DelayUs(20);
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
float CurrentsamplGetCurrent(u8 passage) {
    float current = 0;
    u16 ad_value = 0;
	ad_value = CurrentsamplGetAd(passage);
    current = ((8.7153*ad_value)/65535);
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
        ask[num] = (u8)(current*100);
    } else {
        ask[num] = 0;
    }
    BluetoothSend(0x70,ask[0],ask[1],ask[2],ask[3],ask[4],0,0,0);
}

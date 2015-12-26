


#include "Currentsampl.h"
#include "Delay.h"
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
    
    PE_DDR_DDR5= 1;//EN
    PE_CR1_C15 = 1;
    PE_CR2_C25 = 1;
    
    ADC_CSR &= ~BIT(6);
	ADC_CSR &= ~BIT(4);
	ADC_CR1 |= BIT(1);
	ADC_CR2 |= BIT(1);
	ADC_TDRL = 0x01;
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
    u8 ad_h = 0;
    u8 ad_l = 0;
	ADC_CSR = passage;
	ADC_CR1 = 0x01;
	DelayUs(7);
	ADC_CR1 = ADC_CR1 | 0x01; 
	while((ADC_CSR & 0x80) == 0); 
    ad_h = ADC_DRH;
    ad_l = ADC_DRL;
	ad_value = (ad_h<<8) + ad_l;
    current = ((8.7153*ad_value)/65535);
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
    u8 ad_h = 0;
    u8 ad_l = 0;
	ADC_CSR = passage;
	ADC_CR1 = 0x01;
	DelayUs(7);
	ADC_CR1 = ADC_CR1 | 0x01; 
	while((ADC_CSR & 0x80) == 0); 
    ad_h = ADC_DRH;
    ad_l = ADC_DRL;
	ad_value = (ad_h<<8) + ad_l;
    voltage = ((6.6*ad_value)/65535);
    return voltage; 
}


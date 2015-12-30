
#include "Usbout.h"

/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
void UsboutInit(void) {
    PC_DDR_DDR1= 1;
    PC_CR1_C11 = 1;
    PC_CR2_C21 = 0;//1
    
    PC_DDR_DDR2= 1;
    PC_CR1_C12 = 1;
    PC_CR2_C22 = 0;//1
    
    PC_DDR_DDR3= 1;
    PC_CR1_C13 = 1;
    PC_CR2_C23 = 0;//1
    
    PC_DDR_DDR4= 1;
    PC_CR1_C14 = 1;
    PC_CR2_C24 = 0;//1
    
    PA_DDR_DDR3= 1;
    PA_CR1_C13 = 1;
    PA_CR2_C23 = 0;//1
}

void UsboutSet(u8 chx,u8 data) {
	if(chx == 0x00) {
		TIM1_CCR1L = data; 
	}
	if(chx == 0x01) {
		TIM1_CCR2L = data;  
	}
	if(chx == 0x02) {
		TIM1_CCR3L = data; 
	}
	if(chx == 0x03) {
		TIM1_CCR4L = data; 
	}
	if(chx == 0x04) {
		TIM2_CCR3L = data;      
	}
}
    
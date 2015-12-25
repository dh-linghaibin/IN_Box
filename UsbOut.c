
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
    PC_CR2_C21 = 1;
    
    PC_DDR_DDR2= 1;
    PC_CR1_C12 = 1;
    PC_CR2_C22 = 1;
    
    PC_DDR_DDR3= 1;
    PC_CR1_C13 = 1;
    PC_CR2_C23 = 1;
    
    PC_DDR_DDR4= 1;
    PC_CR1_C14 = 1;
    PC_CR2_C24 = 1;
    
    PA_DDR_DDR3= 1;
    PA_CR1_C13 = 1;
    PA_CR2_C23 = 1;
}


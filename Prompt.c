

#include "Prompt.h"


void PromptInit(void) {
    PD_DDR_DDR4= 1;
    PD_CR1_C14 = 1;
    PD_CR2_C24 = 1;
    
    PA_DDR_DDR4= 1;
    PA_CR1_C14 = 1;
    PA_CR2_C24 = 1;
    
    PE_DDR_DDR0= 0;
    PE_CR1_C10 = 1;
    PE_CR2_C20 = 0;
}


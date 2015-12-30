

#include "Prompt.h"
#include "Eeprom.h"

#define PROMMPT_MOTO    PA_ODR_ODR4
#define IN_POCKET       PE_IDR_IDR0

static u8 prompt_music_flag = 0;

static u8 pocket_flag = 0;

void PromptInit(void) {
    PD_DDR_DDR4= 1;
    PD_CR1_C14 = 1;
    PD_CR2_C24 = 0;//1
    
    PA_DDR_DDR4= 1;
    PA_CR1_C14 = 1;
    PA_CR2_C24 = 0;//1
    
    PE_DDR_DDR0= 0;
    PE_CR1_C10 = 1;
    PE_CR2_C20 = 0;
    if(EepromRead(60) == 0x55) {
        prompt_music_flag = EepromRead(61);
        pocket_flag = EepromRead(62);
    } else {
        EepromWrite(60,0x55);
        EepromWrite(61,0x01);
        EepromWrite(62,0x01);
    }
}

static u8 prompt_music = 0;
static u8 prompt_moto = 0;

void PromptTimeServive(void) {
    static u16 prompt_count = 0;
    static u16 prompt_count_moto = 0;
  
    if(prompt_music > 0) {
        if(prompt_count < 500) {
            prompt_count++;
        } else {
            prompt_count = 0;
            prompt_music--;
        }
        if(prompt_music == 1) {
            TIM2_CCR1L = 0x00;
        }
    }
    
    if(prompt_moto > 0) {
        if(prompt_count_moto < 500) {
            prompt_count_moto++;
        } else {
            prompt_count_moto = 0;
            prompt_moto--;
        }
        if(prompt_count_moto == 1) {
            PROMMPT_MOTO = 0;
        }
    }
}

void PromptSetMusic(u8 data) {
    prompt_music = data;
    TIM2_CCR1L = 0x0f;
}

void PromptSetMoto(u8 data) {
    prompt_moto = data;
    PROMMPT_MOTO = 1;
}

void PromptMucicReflect(void) {
    if(prompt_music_flag == 1) {
        PromptSetMusic(20);
    } else if(prompt_music_flag == 2) {
        PromptSetMoto(10);
    } else if(prompt_music_flag == 3) {
        PromptSetMusic(20);
        PromptSetMoto(20);
    }
}
void PromptMucicSetFlag(u8 cmd) {
    prompt_music_flag = cmd;
    EepromWrite(61,prompt_music_flag);
}

void PromptMusicShowdown(void) {
    TIM2_CCR1L = 0x00;
    PROMMPT_MOTO = 0;
}
/*
»ô¶û ¼ì²â
*/
u8 PromptPocket(void) {
    if(IN_POCKET == 1) {
        return 0x80;
    } else {
        return 0x00;
    }
}

void PromptPocketSetFlag(u8 cmd) {
    pocket_flag = cmd;
    EepromWrite(62,pocket_flag);
}

void PromptPocketReflect(void) {
    if(pocket_flag > 0) {
        if(PromptPocket() == 0x80) {
            if(pocket_flag == 1) {
                PromptSetMusic(30);
            } else if(pocket_flag == 2) {
                PromptSetMoto(10);
            } else if(pocket_flag == 3) {
                PromptSetMusic(30);
                PromptSetMoto(10);
            }
        }
    }
}

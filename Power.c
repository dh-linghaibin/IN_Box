

#include "Power.h"
#include "Eeprom.h"

#define POWER_LEFT      PG_ODR_ODR0
#define POWER_RIGHT     PG_ODR_ODR1

static float power_bat[5] = {0,0,0,0,0};

static float power_in[2] = {0,0};

static u16 power_bank[2] = {0,0};

static u8 power_en_bit[2] = {0,0};

void PowerInit(void) {
    PG_DDR_DDR0 = 1;
    PG_CR1_C10 = 1;
    PG_CR2_C20 = 0;
    
    PG_DDR_DDR1 = 1;
    PG_CR1_C11 = 1;
    PG_CR2_C21 = 0;
    
    if(EepromRead(70) != 0x55) {
        EepromWrite(70,0x55);
        EepromWrite(71,0x01);
        EepromWrite(72,0x01);
    }
    power_en_bit[0] = EepromRead(71);
    power_en_bit[1] = EepromRead(72);
    POWER_LEFT = power_en_bit[0];
    POWER_RIGHT = power_en_bit[1];
}

void PowerSetBit(u8 bit, u8 cmd) {
    power_en_bit[bit] = cmd;
    POWER_LEFT = power_en_bit[0];
    POWER_RIGHT = power_en_bit[1];
}

void PowerAdd(u8 num, float power) {
    power_bat[num] += power;
}

void PowerSub(u8 num, float power) {
    power_in[num] += power;
}

float PowerGet(u8 num) {
    return power_bat[num];
}

float PowerGetIn(u8 num) {
    return power_in[num];
}

float PowerGetAll(void) {
    float bat = 0;
    u8 add_i = 0;
    for(add_i = 0; add_i < 5; add_i++) {
        bat += power_bat[add_i];
    }
    bat -= power_in[0];
    bat -= power_in[1];
    if(bat < 0) {
        bat = 0;
    }
    return bat;
}

void PowerBankSet(u8 num, u16 power) {
    power_bank[num] += power;
}


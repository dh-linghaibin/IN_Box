

#include "Power.h"
#include "Eeprom.h"

#define POWER_LEFT      PE_ODR_ODR1
#define POWER_RIGHT     PG_ODR_ODR1

static float power_bat[5] = {0,0,0,0,0};

static float power_in[2] = {0,0};

static u16 power_bank[2] = {0,0};

static u8 power_en_bit[2] = {0,0};

static u16 power_locking[5] = {1000,1000,1000,1000,1000};

void PowerInit(void) {
    u8 read_i = 0;
    PE_DDR_DDR1 = 1;
    PE_CR1_C11 = 1;
    PE_CR2_C21 = 0;
    
    PG_DDR_DDR1 = 1;
    PG_CR1_C11 = 1;
    PG_CR2_C21 = 0;
    
    if(EepromRead(70) != 0x55) {
        EepromWrite(70,0x55);
        EepromWrite(71,0x01);
        EepromWrite(72,0x01);
        for(read_i = 0; read_i < 5; read_i++) {
            EepromWrite(72+(read_i*2),0x03);
            EepromWrite(73+(read_i*2),0xe8);
        }
    }
    power_en_bit[0] = EepromRead(71);
    power_en_bit[1] = EepromRead(72);
    if(power_en_bit[0] == 0) {
        POWER_LEFT = 1;
    } else {
        POWER_LEFT = 0;
    }
    if(power_en_bit[1] == 0) {
        POWER_RIGHT = 0;
    } else {
        POWER_RIGHT = 1;
    }
    for(read_i = 0; read_i < 5; read_i++) {
        power_locking[read_i] = EepromRead(72+(read_i*2));
        power_locking[read_i] |= (EepromRead(73+(read_i*2)) << 8);
    }
}

void PowerSetBit(u8 bit, u8 cmd) {
    power_en_bit[bit] = cmd;
    EepromWrite(71,power_en_bit[0]);
    EepromWrite(72,power_en_bit[1]);
    if(power_en_bit[0] == 0) {
        POWER_LEFT = 1;
    } else {
        POWER_LEFT = 0;
    }
    if(power_en_bit[1] == 0) {
        POWER_RIGHT = 0;
    } else {
        POWER_RIGHT = 1;
    }
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

void PowerLockingSet(u8 num, u8 percentage) {
    power_locking[num] = (u16)(power_bank[0]*(percentage/100) );
    EepromWrite(72+(num*2),(u8)power_locking[num]);
    EepromWrite(73+(num*2),(u8)(power_locking[num] >> 8));
}

u8 PowerLockingCloose(u8 num) {
    if(power_bat[num] > power_locking[num]) {
        return 0x80;
    } else {
        return 0x00;
    }
}

u8 PowerCountBlank(u8 cmd) {
    return (u8)( (power_bank[cmd] - PowerGetAll())*100 /power_bank[cmd] );
}
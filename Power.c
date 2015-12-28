

#include "Power.h"

static float power_bat[5] = {0,0,0,0,0};

static float power_in[2] = {0,0};

static u16 power_bank[2] = {0,0};

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

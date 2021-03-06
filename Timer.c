

#include "Timer.h"

/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
void TimerInit(void) {
    TIM4_IER = 0x00;//       
    TIM4_EGR = 0x01;// 
    TIM4_PSCR = 0x07;// 计数器时钟=主时钟/128=16MHZ/128
    TIM4_ARR = 0xFA;// 设定重装载时的寄存器值，255是最大值
    //TIM4_CNTR = 0x00;// 设定计数器的初值
    // 定时周期=(ARR+1)*64=16320uS
    TIM4_IER = 0x01;//   
    TIM4_CR1 = 0x01;  
}
/*!
 @property  
 @abstract  
 */
static u8 timer_sec_flag = 0;
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
u8 TimerGetSecFlag(void) {
    return timer_sec_flag;
}
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
void TimerClearSecFlag(void) {
    timer_sec_flag = 0x00;
}

#pragma vector=0x19
__interrupt void TIM4_UPD_OVF_IRQHandler(void)
{
    static u16 count_time = 0;
    //static u8 count_sec = 0;
    TIM4_SR = 0x00;
    
    if(count_time < 500) {
        count_time++;
    } else {
        count_time = 0;
        timer_sec_flag = 0x80;
    }
    
    return;
}
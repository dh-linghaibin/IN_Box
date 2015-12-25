

#include "Show.h"
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
void ShowInit(void) {
    
    PD_DDR_DDR0 = 1;
    PD_CR1_C10 = 1;
    PD_CR2_C20 = 1;
    
    PD_DDR_DDR2 = 1;
    PD_CR1_C12 = 1;
    PD_CR2_C22 = 1;
    
    PD_DDR_DDR3 = 1;
    PD_CR1_C13 = 1;
    PD_CR2_C23 = 1;
    
    TIM1_EGR |= 0x01;   //重新初始化TIM1 
	TIM1_CR1 = 0x00;   //B7(0)可以直接写入 B65(00)边缘对齐模式B4(0)向上计数B3(0)计数器不停止发生更新事件
	TIM1_RCR = 0x00;  
	TIM1_PSCRH =0;     //设定预分频为,16分频 1M
	TIM1_PSCRL =0x80;  //PWM的时钟 影响周期

	TIM1_CCMR1 = 0x60; //PWM模式1,CC1配置入输出
	TIM1_CCMR2 = 0x60; //PWM模式1,CC1配置入输出
	TIM1_CCMR3 = 0x60; //PWM模式1,CC1配置入输出
	TIM1_CCMR4 = 0x60; //PWM模式1,CC1配置入输出
	
	TIM1_ARRH = 0;     //设定重装载值
	TIM1_ARRL = 0xFF;  //PWM的周期 
	
	TIM1_CCR1H = 0;
	TIM1_CCR1L = 0;  // 占空比值
	
	TIM1_CCR2H = 0;
	TIM1_CCR2L = 0;  // 占空比值
	
	TIM1_CCR3H = 0;
	TIM1_CCR3L = 0;  // 占空比值
	
	TIM1_CCR4H = 0;
	TIM1_CCR4L = 0;  // 占空比值
	
	TIM1_CR1 |= 0x01; //使能TIM1计数器
	TIM1_BKR |= 0x80; 
    
    	/*PWM初始化*/
	TIM2_IER = 0x00;         /*PWM中断主要在这里进行调整*/
	TIM2_CCMR3 = 0x60;  //pwm模式2
	TIM2_CCMR2 = 0x70;  //pwm模式2
	TIM2_CCMR1 = 0x70;  //pwm模式2
	
	TIM2_CCER1 |= 0x13;
	TIM2_CCER2 = 3;  		//CC2引脚使能
	
	TIM2_PSCR = 0x05;         //8^1 分频为1M 
	TIM2_ARRH = 0x01;
	TIM2_ARRL = 0x80;        //每500us中断一次1F4-2k,100-10k
	
	TIM2_CCR3H = 0x00;
	TIM2_CCR3L = 0;        //设置占空比，即计数器到多少后
	
	TIM2_CCR2H = 0x00;
	TIM2_CCR2L = 0x00;        //设置占空比，即计数器到多少后
	
	TIM2_CCR1H = 0x00;
	TIM2_CCR1L = 0x00;        //设置占空比，即计数器到多少后
	
	TIM2_CR1 = 0x04;
	TIM2_CR1 |= BIT(0);              //计数器使能，开始计数
	
	/*PWM初始化*/
	TIM3_IER = 0x00;         /*PWM中断主要在这里进行调整*/
	TIM3_CCMR2 = 0x70;  //pwm模式2
	TIM3_CCMR1 = 0x70;  //pwm模式2
	
	TIM3_CCER1 |= 0x11;
	
	TIM3_PSCR = 0x05;         //8^1 分频为1M 
	TIM3_ARRH = 0x01;
	TIM3_ARRL = 0x80;        //每500us中断一次1F4-2k,100-10k
	
	TIM3_CCR2H = 0x00;
	TIM3_CCR2L = 0x00;        //设置占空比，即计数器到多少后
	
	TIM3_CCR1H = 0x00;
	TIM3_CCR1L = 0x00;        //设置占空比，即计数器到多少后
	
	TIM3_CR1 = 0x04;
	TIM3_CR1 |= BIT(0);              //计数器使能，开始计数
}
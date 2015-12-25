

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
    
    TIM1_EGR |= 0x01;   //���³�ʼ��TIM1 
	TIM1_CR1 = 0x00;   //B7(0)����ֱ��д�� B65(00)��Ե����ģʽB4(0)���ϼ���B3(0)��������ֹͣ���������¼�
	TIM1_RCR = 0x00;  
	TIM1_PSCRH =0;     //�趨Ԥ��ƵΪ,16��Ƶ 1M
	TIM1_PSCRL =0x80;  //PWM��ʱ�� Ӱ������

	TIM1_CCMR1 = 0x60; //PWMģʽ1,CC1���������
	TIM1_CCMR2 = 0x60; //PWMģʽ1,CC1���������
	TIM1_CCMR3 = 0x60; //PWMģʽ1,CC1���������
	TIM1_CCMR4 = 0x60; //PWMģʽ1,CC1���������
	
	TIM1_ARRH = 0;     //�趨��װ��ֵ
	TIM1_ARRL = 0xFF;  //PWM������ 
	
	TIM1_CCR1H = 0;
	TIM1_CCR1L = 0;  // ռ�ձ�ֵ
	
	TIM1_CCR2H = 0;
	TIM1_CCR2L = 0;  // ռ�ձ�ֵ
	
	TIM1_CCR3H = 0;
	TIM1_CCR3L = 0;  // ռ�ձ�ֵ
	
	TIM1_CCR4H = 0;
	TIM1_CCR4L = 0;  // ռ�ձ�ֵ
	
	TIM1_CR1 |= 0x01; //ʹ��TIM1������
	TIM1_BKR |= 0x80; 
    
    	/*PWM��ʼ��*/
	TIM2_IER = 0x00;         /*PWM�ж���Ҫ��������е���*/
	TIM2_CCMR3 = 0x60;  //pwmģʽ2
	TIM2_CCMR2 = 0x70;  //pwmģʽ2
	TIM2_CCMR1 = 0x70;  //pwmģʽ2
	
	TIM2_CCER1 |= 0x13;
	TIM2_CCER2 = 3;  		//CC2����ʹ��
	
	TIM2_PSCR = 0x05;         //8^1 ��ƵΪ1M 
	TIM2_ARRH = 0x01;
	TIM2_ARRL = 0x80;        //ÿ500us�ж�һ��1F4-2k,100-10k
	
	TIM2_CCR3H = 0x00;
	TIM2_CCR3L = 0;        //����ռ�ձȣ��������������ٺ�
	
	TIM2_CCR2H = 0x00;
	TIM2_CCR2L = 0x00;        //����ռ�ձȣ��������������ٺ�
	
	TIM2_CCR1H = 0x00;
	TIM2_CCR1L = 0x00;        //����ռ�ձȣ��������������ٺ�
	
	TIM2_CR1 = 0x04;
	TIM2_CR1 |= BIT(0);              //������ʹ�ܣ���ʼ����
	
	/*PWM��ʼ��*/
	TIM3_IER = 0x00;         /*PWM�ж���Ҫ��������е���*/
	TIM3_CCMR2 = 0x70;  //pwmģʽ2
	TIM3_CCMR1 = 0x70;  //pwmģʽ2
	
	TIM3_CCER1 |= 0x11;
	
	TIM3_PSCR = 0x05;         //8^1 ��ƵΪ1M 
	TIM3_ARRH = 0x01;
	TIM3_ARRL = 0x80;        //ÿ500us�ж�һ��1F4-2k,100-10k
	
	TIM3_CCR2H = 0x00;
	TIM3_CCR2L = 0x00;        //����ռ�ձȣ��������������ٺ�
	
	TIM3_CCR1H = 0x00;
	TIM3_CCR1L = 0x00;        //����ռ�ձȣ��������������ٺ�
	
	TIM3_CR1 = 0x04;
	TIM3_CR1 |= BIT(0);              //������ʹ�ܣ���ʼ����
}
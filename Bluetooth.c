/*!
 @header     文件名
 @abstract   简介，概念
 @discussion 用途，方法
 @copyright  版权
 */
#include "Bluetooth.h"
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
#define BLUE_EN PD_ODR_ODR6     

void BluetoothInit(void) {
    PA_DDR_DDR2 = 1;//BRTS
    PA_CR1_C12 = 1;
    PA_CR2_C22 = 0;

    PA_DDR_DDR1 = 1;//BCTS
    PA_CR1_C11 = 1;
    PA_CR2_C21 = 0;
    
    PD_DDR_DDR7 = 1;//EN
    PD_CR1_C17 = 1;
    PD_CR2_C27 = 1;
    
    PD_DDR_DDR6 = 0;
    PD_CR1_C16 = 1;
    PD_CR2_C26 = 0;
   
    PD_DDR_DDR5 = 1;
    PD_CR1_C15 = 1;
    PD_CR2_C25 = 1;

    UART2_CR1=0x00;
    UART2_CR2=0x00;
    UART2_CR3=0x00; 
    UART2_BRR2=0x02;//02 0a
    UART2_BRR1=0x68;//68 08
    UART2_CR2=0x2c;//允许接收，发送，开接收中断
    BLUE_EN = 0;
}
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
void BluetoothSend(u8 cmd,
               u8 date0,u8 data1,
               u8 data2,u8 data3,
               u8 data4,u8 data5,
               u8 data6,u8 data7)
{
    u8 data_txd[13];
	u16 jy_data = 0;//计算校验结果
	u8 count_i = 0;
    
    data_txd[0] = 0x3a;
    data_txd[1] = cmd;
    data_txd[2] = date0;
	data_txd[3] = data1;
    data_txd[4] = data2;
    data_txd[5] = data3;
    data_txd[6] = data4;
    data_txd[7] = data5;
    data_txd[8] = data6;
    data_txd[9] = data7;
    data_txd[10] = 0x00;
    data_txd[11] = 0x00;
    data_txd[12] = 0x0a;
    
	for(count_i = 1;count_i < 10;count_i++)//累加
	{
		jy_data += data_txd[count_i];
	}
	data_txd[10] = jy_data;
	data_txd[11] = jy_data >> 8;
    
	for(count_i = 0;count_i < 13;count_i++)
	{
        while((UART2_SR & 0x80) == 0x00);    // 等待数据的传送  
        UART2_DR = data_txd[count_i]; 
	}
}
/*!
 @property  
 @abstract  
 */
static u8 blue_rx_data[13];
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
u8 BluetoothGetDataRx(u8 num) {
    return blue_rx_data[num];
}
/*!
 @property  
 @abstract  
 */
static u8 blue_rx_flag = 0;
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
u8 BluetoothGetFlag(void) {
    return blue_rx_flag;
}
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
void BluetoothClearFlag(void) {
    blue_rx_flag = 0x00;
}
/*!
 @method     
 @abstract   
 @discussion 
 @param   
 */
u8 BluetoothCheck(void)
{  
	u16 add_data = 0x00;
	u8 count_i;
	for(count_i = 1;count_i < 9;count_i++) {
		add_data += blue_rx_data[count_i];
	}
	if(blue_rx_data[10] == (add_data&0x00ff)) {
		if(blue_rx_data[11] == (add_data >> 8)) {
			return 0x80;
		}
	}
	return 0x00;
}

static u8 blue_start = 0;
static u8 blue_add = 0;

void BluetoothTimeServive(void) {
    static u16 count = 0;
    if( (blue_start == 0x80)||(blue_add > 0) )
    {
        if(count < 5000) {
            count++;
        } else {
            count = 0;
            blue_start = 0x00;
            blue_add = 0x00;
        }
        
    } else {
        count = 0;
    }
}

#pragma vector=0x16
__interrupt void UART2_TX_IRQHandler(void)
{
   
}
#pragma vector=0x17
__interrupt void UART2_RX_IRQHandler(void)
{
    u8 data_rx;
    data_rx = UART2_DR;
    while((UART2_SR & 0x80) == 0x00);// 等待数据的传送  
    if(data_rx == 0x3a)
    {
        blue_start = 0x80;
    }
    if(blue_start == 0x80)
    {
        blue_rx_data[blue_add] = data_rx;
        blue_add++;
        if(blue_add == 13)
        {
            if(data_rx == 0x0a)
            {
                blue_start = 0x00;
                blue_add = 0;
                blue_rx_flag = 0x80;//数据接收完成
            }
        }
    }
    return;
}

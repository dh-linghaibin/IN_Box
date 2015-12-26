
#include "Com.h"
#include "Delay.h"

void ComInit(void) {
    //ring
    PE_DDR_DDR2= 0;
    PE_CR1_C12 = 0;
    PE_CR2_C22 = 1;
    //watch
    PC_DDR_DDR6= 0;
    PC_CR1_C16 = 0;
    PC_CR2_C26 = 1;
    
	EXTI_CR2 |= BIT(1);//开启PC口中断
	EXTI_CR2 &= ~BIT(0);
	
	EXTI_CR1 |= BIT(5);//开启PC口中断
	EXTI_CR1 &= ~BIT(4);
}

#define COM_BIT_OUT 	PE_ODR_ODR2
#define COM_BIT_IN 	 	PE_IDR_IDR2
#define COM_BIT_DR 		PE_DDR_DDR2
#define COM_BIT_INT 	PE_CR2_C22

u8 ComSend(u8 data[])
{
	u16 wait = 0;
	u8 data_t = 0;//保存临时值
	u8 i = 0,j = 0;
	COM_BIT_DR = 1;//设置为输出
	COM_BIT_OUT = 1;
	COM_BIT_OUT = 0;
	DelayUs(200);//拉低20ms说明总线开始
	COM_BIT_DR = 0;//设置为输入
	DelayUs(10);//拉低20ms说明总线开始
	while(COM_BIT_IN == 1)//等待从机拉高
	{
		wait++;
		if(wait > 1530)//超时，退出
		{
			COM_BIT_DR = 1;//设置为输出
			COM_BIT_OUT = 1;
			return 0;
		}
	}
	while(COM_BIT_IN == 0);//等待从机拉高完成
	COM_BIT_DR = 1;//设置为输出
	for(j = 0;j < 5;j++)
	{
		data_t = data[j];
		for(i=0;i<8;i++)
		{
			COM_BIT_OUT = 0;
			if(data_t&0x80)
			{
				DelayUs(160);
			}
			else 
			{
				DelayUs(80);
			}
			COM_BIT_OUT = 1;
			DelayUs(80);
			data_t<<=1;
		}
	}
	DelayUs(120);
	COM_BIT_OUT = 1;
	COM_BIT_DR = 0;//设置为输入
	return 0;
}

u8 ComRead(u8 data_s[])
{
    static u16 wait_r = 0;//统计下拉时间
	u8 i = 0,j = 0;
	u8 data = 0;
	wait_r = 0;

	//COM_BIT_DR1 = 0;//设置为输入
	while(COM_BIT_IN == 0)
	{
		if(wait_r < 100)
		{
			wait_r++;
		}
		else 
		{
			return 0x44;
		}
	}
	if(wait_r > 55)
	{
		wait_r = 0;
		COM_BIT_DR = 1;//设置为输出
		COM_BIT_OUT = 0;
		DelayUs(1);
		COM_BIT_DR = 0;//设置为输入
		DelayUs(10);
		//开始接受数据
		for(j = 0;j < 5;j++)
		{
			for(i=0;i<8;i++)  
			{
				data <<= 1; 
				while(COM_BIT_IN == 1)
				{
					if(wait_r < 500)
					{
						wait_r++;
					}
					else
					{
						return 0x44;
					}
				}
				wait_r = 0;
				while(COM_BIT_IN == 0)
				{
					if(wait_r < 500)
					{
						wait_r++;
					}
					else
					{
						return 0x44;
					}
				}
				if(wait_r > 40)//为1
				{
					data|=0x01;  
				}
				wait_r = 0;	
			}
			data_s[j] = data;
			data = 0;
		}
		if(data_s[4] == (u8)(data_s[0]+data_s[1]+data_s[2]+data_s[3]))//累加校验
		{
			if(data_s[4] != 0)
			{
				return 0x88;
			}
			else
			{
				return 0x44;
			}
		}
		else
		{
			return 0x44;
		}
	}
	else//时间不对 推出
	{
		return 0x44;
	}
}

volatile u8 com_t_data[5] = {0,0,0,0,0};//前拨

u8 com_send_cm(u8 cmd,u8 par1,u8 par2,u8 par3)
{
	u16 com_check = 0;//保存累加校验值
	com_check = cmd+par1+par2+par3;
	com_t_data[4] = com_check;
	com_t_data[0] = cmd;
	com_t_data[1] = par1;
	com_t_data[2] = par2;
	com_t_data[3] = par3;
	ComSend(com_t_data);
}




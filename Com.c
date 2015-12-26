
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
    
	EXTI_CR2 |= BIT(1);//����PC���ж�
	EXTI_CR2 &= ~BIT(0);
	
	EXTI_CR1 |= BIT(5);//����PC���ж�
	EXTI_CR1 &= ~BIT(4);
}

#define COM_BIT_OUT 	PE_ODR_ODR2
#define COM_BIT_IN 	 	PE_IDR_IDR2
#define COM_BIT_DR 		PE_DDR_DDR2
#define COM_BIT_INT 	PE_CR2_C22

u8 ComSend(u8 data[])
{
	u16 wait = 0;
	u8 data_t = 0;//������ʱֵ
	u8 i = 0,j = 0;
	COM_BIT_DR = 1;//����Ϊ���
	COM_BIT_OUT = 1;
	COM_BIT_OUT = 0;
	DelayUs(200);//����20ms˵�����߿�ʼ
	COM_BIT_DR = 0;//����Ϊ����
	DelayUs(10);//����20ms˵�����߿�ʼ
	while(COM_BIT_IN == 1)//�ȴ��ӻ�����
	{
		wait++;
		if(wait > 1530)//��ʱ���˳�
		{
			COM_BIT_DR = 1;//����Ϊ���
			COM_BIT_OUT = 1;
			return 0;
		}
	}
	while(COM_BIT_IN == 0);//�ȴ��ӻ��������
	COM_BIT_DR = 1;//����Ϊ���
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
	COM_BIT_DR = 0;//����Ϊ����
	return 0;
}

u8 ComRead(u8 data_s[])
{
    static u16 wait_r = 0;//ͳ������ʱ��
	u8 i = 0,j = 0;
	u8 data = 0;
	wait_r = 0;

	//COM_BIT_DR1 = 0;//����Ϊ����
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
		COM_BIT_DR = 1;//����Ϊ���
		COM_BIT_OUT = 0;
		DelayUs(1);
		COM_BIT_DR = 0;//����Ϊ����
		DelayUs(10);
		//��ʼ��������
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
				if(wait_r > 40)//Ϊ1
				{
					data|=0x01;  
				}
				wait_r = 0;	
			}
			data_s[j] = data;
			data = 0;
		}
		if(data_s[4] == (u8)(data_s[0]+data_s[1]+data_s[2]+data_s[3]))//�ۼ�У��
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
	else//ʱ�䲻�� �Ƴ�
	{
		return 0x44;
	}
}

volatile u8 com_t_data[5] = {0,0,0,0,0};//ǰ��

u8 com_send_cm(u8 cmd,u8 par1,u8 par2,u8 par3)
{
	u16 com_check = 0;//�����ۼ�У��ֵ
	com_check = cmd+par1+par2+par3;
	com_t_data[4] = com_check;
	com_t_data[0] = cmd;
	com_t_data[1] = par1;
	com_t_data[2] = par2;
	com_t_data[3] = par3;
	ComSend(com_t_data);
}




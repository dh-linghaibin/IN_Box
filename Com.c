
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
    PC_CR2_C26 = 0;
    
	EXTI_CR2 |= BIT(1);//����PC���ж�
	EXTI_CR2 &= ~BIT(0);
	
	EXTI_CR1 |= BIT(5);//����PC���ж�
	EXTI_CR1 &= ~BIT(4);
}

#define COM_BIT_OUT 	PE_ODR_ODR2
#define COM_BIT_IN 	 	PE_IDR_IDR2
#define COM_BIT_DR 		PE_DDR_DDR2
#define COM_BIT_INT 	PE_CR2_C22

u8 ComSendRing(u8 data[])
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

u8 ComReadRing(u8 data_s[])
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
	if(wait_r > 45)
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
				if(wait_r > 35)//Ϊ1
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

void ComSendCmdRing(u8 cmd,u8 par1,u8 par2,u8 par3)
{
    u8 com_t_data[5] = {0,0,0,0,0};//ǰ��
	u16 com_check = 0;//�����ۼ�У��ֵ
	com_check = cmd+par1+par2+par3;
	com_t_data[4] = com_check;
	com_t_data[0] = cmd;
	com_t_data[1] = par1;
	com_t_data[2] = par2;
	com_t_data[3] = par3;
    INTOFF
	ComSendRing(com_t_data);
    INTEN
}

static u8 com_r_date_ring[5];
static u8 rs_ok_ring = 0;//whether it has received data

u8 ComGetRingData(u8 num) {
    return com_r_date_ring[num];
}

u8 ComGetRingFlag(void) {
    return rs_ok_ring;
}

void ComClearRingFlag(void) {
    rs_ok_ring = 0x00;
}

#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void)
{
    INTOFF
	if(ComReadRing(com_r_date_ring) == 0x88) {
		rs_ok_ring = 0x80;
	}
    INTOFF
}


#define COM_BIT_OUT1 		PC_ODR_ODR6
#define COM_BIT_IN1 	 	PC_IDR_IDR6
#define COM_BIT_DR1 		PC_DDR_DDR6
#define COM_BIT_INT1 		PC_CR2_C26

u8 ComSendWatch(u8 data[]) {
	u16 wait = 0;
	u8 data_t = 0;//������ʱֵ
	u8 i = 0,j = 0;
	
	COM_BIT_INT1 = 0;//�ж�
	COM_BIT_DR1 = 1;//����Ϊ���
	COM_BIT_OUT1 = 0;
	DelayUs(250);//����20ms˵�����߿�ʼ 250
	COM_BIT_DR1 = 0;//����Ϊ����
	DelayUs(1);
	while(COM_BIT_IN1 == 1)//�ȴ��ӻ�����
	{
		if(wait < 200)
		{
			wait++;
		}
		else//��ʱ���˳�
		{
			//COM_BIT_INT1 = 1;//�ж�
			return 0;
		}
	}
	wait = 0;
	while(COM_BIT_IN1 == 0)
	{
		if(wait < 200)
		{
			wait++;
		}
		else//��ʱ���˳�
		{
			return 0;
		}
	}
	COM_BIT_DR1 = 1;//����Ϊ���
	for(j = 0;j < 6;j++)
	{
		data_t = data[j];
		for(i=0;i<8;i++)
		{
			COM_BIT_OUT1 = 0;
			if(data_t&0x80)
			{
				DelayUs(120);
			}
			else 
			{
				DelayUs(40);
			}
			COM_BIT_OUT1 = 1;
			DelayUs(20);
			data_t<<=1;
		}
	}
	DelayUs(60);
	
	COM_BIT_OUT1 = 1;
	
	//COM_BIT_INT1 = 1;//�ж�
	COM_BIT_DR1 = 0;//����Ϊ����
	return 0x88;
}

u8 ComReadWatch(u8 data_s[])
{
    static u16 wait_r1 = 0;//ͳ������ʱ��
	u8 i = 0,j = 0;
	u8 data = 0;
	wait_r1 = 0;

	//COM_BIT_DR1 = 0;//����Ϊ����
	while(COM_BIT_IN1 == 0)
	{
		if(wait_r1 < 100)
		{
			wait_r1++;
		}
		else 
		{
			return 0x44;
		}
	}
	if(wait_r1 > 55)
	{
		wait_r1 = 0;
		COM_BIT_DR1 = 1;//����Ϊ���
		COM_BIT_OUT1 = 0;
		DelayUs(1);
		COM_BIT_DR1 = 0;//����Ϊ����
		DelayUs(10);
		//��ʼ��������
		for(j = 0;j < 6;j++)
		{
			for(i=0;i<8;i++)  
			{
				data <<= 1; 
				while(COM_BIT_IN1 == 1)
				{
					if(wait_r1 < 200)
					{
						wait_r1++;
					}
					else
					{
						return 0x44;
					}
				}
				wait_r1 = 0;
				while(COM_BIT_IN1 == 0)
				{
					if(wait_r1 < 200)
					{
						wait_r1++;
					}
					else
					{
						return 0x44;
					}
				}
				if(wait_r1 > 40)//Ϊ1
				{
					data|=0x01;  
				}
				wait_r1 = 0;	
			}
			data_s[j] = data;
			data = 0;
		}
		if(data_s[5] == (u8)(data_s[0]+data_s[1]+data_s[2]+data_s[3]+data_s[4]))//�ۼ�У��
		{
			if(data_s[5] != 0)
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

void ComSendCmdWatch(u8 cmd,u8 par1,u8 par2,u8 par3,u8 par4)
{
    u8 com_t_data[6] = {0,0,0,0,0,0};//ǰ��
	//u16 com_check = 0;//�����ۼ�У��ֵ
	//com_check = cmd+par1+par2+par3+par4;
	com_t_data[0] = cmd; //cmd
	com_t_data[1] = par1;
	com_t_data[2] = par2;
	com_t_data[3] = par3;
    com_t_data[4] = par4;
    com_t_data[5] = com_t_data[0]+com_t_data[1]+com_t_data[2]
                                    +com_t_data[3]+com_t_data[4];
    INTOFF
	ComSendWatch(com_t_data);
    INTEN
}

static u8 com_r_date_watch[5];
static u8 rs_ok_watch = 0;//whether it has received data

u8 ComGetWatchData(u8 num) {
    return com_r_date_watch[num];
}

u8 ComGetWatchFlag(void) {
    return rs_ok_watch;
}

void ComClearWatchFlag(void) {
    rs_ok_watch = 0x00;
}

#pragma vector=7
__interrupt void EXTI_PORTC_IRQHandler(void)
{
    INTOFF
    if(ComReadWatch(com_r_date_watch) == 0x88) {
        rs_ok_watch = 0x80;
    }
    INTEN
}


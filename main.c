#include "main.h"

int main(void)
{
	OSCCON=0xF0;   // ����32Mhz����

	DEVICE_Initial();
	
	
	while(1)
	{
		Dispatch_2ms();
		GIE = 1;
	}
}

void IO_Initial(void)
{
	TRISC = 0xB0; 	//0 for output,1 for input		
	TRISB = 0xFF;
	TRISA = 0xFF;
   
	ANSELA = 0xF0;	//1Ϊģ�⣬0Ϊ����
	ANSELB = 0xFF;
	ANSELC = 0xF0;

}

void Timer0_Initial(void)
{
	//ѡ��ʱ��ԴΪFosc/4��Fosc=32MHz
	//ͨ������PSAλ����Ԥ��Ƶ�������Timer0
	//����Ԥ��Ƶϵ��1:64
	OPTION_REG = 0x81;
	//��ʱ��ֵ
	TMR0 = 6;//��ʱʱ��250��������2ms
	//��ʼ���ж�
	TMR0IF = 0;
	TMR0IE = 1;
	PEIE   = 1;
	GIE    = 1;
}

void TMR2_Initial(void)
{
	T1CON=0x20;// Ԥ��ƵΪ4 ���ƵΪ2
	TMR1=0;
	TMR1IF=0;
	TMR1IE=0;
	TMR1H = 0;
	TMR1L = 0;
	
	//PR1=249;
//	CCPTMRS=0XCF;
	CCP3CON=0X05;
	CCP3IE = 1;
	CCP3IF = 0;
	CCPR3L=0;
	CCPR3H=0;
	TMR1ON=1;


}

int 	DEVICE_Initial(void)
{
	
	IO_Initial();
	TMR2_Initial();
	Timer0_Initial();

}


void interrupt ISR(void)
{
	if(TMR0IF==1)
	{
		TMR0 = 6;			
		TMR0IF = 0;
		Flag_2ms = 1;
		
	}
	if(CCP3IF == 1)
	{
		CPP++;
	//	tim2 = ((CCPR3H<<8)|CCPR3L);

	//	CCPR3H = 0;
	//	CCPR3L = 0;	
		CCP3IF = 0;	
		//TMR1H = 0;
	//	TMR1L = 0;
		//CCP3CON=0X05;
		if(CPP == 1)
		{
			tim2 = ((CCPR3H<<8)|CCPR3L);
			CCP3CON=0X04;
		//	CPP = 0;
			TMR1H = 0;
			TMR1L = 0;
		}
		else
		{
			tim3 = ((CCPR3H<<8)|CCPR3L);
			CCP3CON=0X05;
			CPP = 0;
		}
		CCPR3H = 0;
		CCPR3L = 0;		

	//	CPP++;
		
	}
}


void	Dispatch_2ms(void)
{
	if(Flag_2ms == 0)	return;
		
	Flag_2ms = 0;

	ClrWdt();
//	CM1ON();
	if((cTemp >= 3)&&(cTemp <5))
	{
		
		CM1OFF();
		cTemp++;
		
	}
	else if(cTemp >=5)
	{
		cTemp=0;	
	}
	else 
	{
		CM1ON();
		cTemp++;
	}
}
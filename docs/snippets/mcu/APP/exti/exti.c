#include "exti.h"
#include "led.h"
#include "SysTick.h"
//#include "beep.h"
#include "key.h"
#include "tftlcd.h"
#include "ffttest.h"
#include "usart.h"
#include "my_lcd.h"
#include "spi.h"
#include "ffttest.h"

/*******************************************************************************
*            : My_EXTI_Init
* 		   : ⲿжϳʼ
*              : 
*              : 
*******************************************************************************/
void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	
	
	//EXTI0 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0жͨ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//ռȼ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//ȼ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨʹ
	NVIC_Init(&NVIC_InitStructure);	//ָĲʼVICĴ
	
	
	//EXTI2 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI2жͨ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//ռȼ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//ȼ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨʹ
	NVIC_Init(&NVIC_InitStructure);	//ָĲʼVICĴ
	
	
	//EXTI3 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3жͨ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//ռȼ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//ȼ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨʹ
	NVIC_Init(&NVIC_InitStructure);	//ָĲʼVICĴ
	
	//EXTI4 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4жͨ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//ռȼ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//ȼ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨʹ
	NVIC_Init(&NVIC_InitStructure);	//ָĲʼVICĴ
	
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}

/*******************************************************************************
*            : EXTI0_IRQHandler
* 		   : ⲿж0
*              : 
*              : 
*******************************************************************************/
uint8_t ADDMODE;
uint8_t SETPHASE;
uint8_t WAVEPHASE=1;
uint8_t Deal_Phase(uint8_t phase_in)
{
	float phase_f=phase_in*256.0/180;
	uint8_t phase_out=phase_f;
	if(phase_out==0x00||phase_out==0x13||phase_out==0x17||phase_out==0x88||phase_out==0x99)
	{
		phase_out++;
	}
	return phase_out;
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==1)
	{
		delay_ms(10);
		if(K_UP==1)
		{
//			led2=0;
			switch(ADDMODE)
			{
				case 0:
					SETPHASE=((SETPHASE+5>180)?(SETPHASE+5-180):(SETPHASE+5));
					break;
				case 1:
					SETPHASE=((SETPHASE+10>180)?(SETPHASE+10-180):(SETPHASE+10));
					break;
				case 2:
					SETPHASE=((SETPHASE+20>180)?(SETPHASE+20-180):(SETPHASE+20));
					break;
				case 3:
					SETPHASE=((SETPHASE+1>180)?(SETPHASE+1-180):(SETPHASE+1));
					break;
				default: SETPHASE=SETPHASE;
			}
			LCD_ShowNum(150,59,SETPHASE,3,24);
			WAVEPHASE=Deal_Phase(SETPHASE);
		}	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

/*******************************************************************************
*            : EXTI3_IRQHandler
* 		   : ⲿж3
*              : 
*              : 
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)==1)
	{
		delay_ms(10);
		if(K_DOWN==0)
		{	
//			led2=1;
			switch(ADDMODE)
			{
				case 0:
					SETPHASE=((SETPHASE<5)?(SETPHASE+180-5):(SETPHASE-5));
					break;
				case 1:
					SETPHASE=((SETPHASE<10)?(SETPHASE+180-10):(SETPHASE-10));
					break;
				case 2:
					SETPHASE=((SETPHASE<20)?(SETPHASE+180-20):(SETPHASE-20));
					break;
				case 3:
					SETPHASE=((SETPHASE<1)?(SETPHASE+180-1):(SETPHASE-1));
					break;
				default: SETPHASE=SETPHASE;
			}
			LCD_ShowNum(150,59,SETPHASE,3,24);
			WAVEPHASE=Deal_Phase(SETPHASE);
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/*******************************************************************************
*            : EXTI2_IRQHandler
* 		   : ⲿж2
*              : 
*              : 
*******************************************************************************/
extern uint8_t FREA,FREB,KINDA,KINDB;
//extern uint16_t SPI_FPGA[128];
float RESULT1[128],RESULT2[128];
extern uint16_t SPI_FPGA1[128],SPI_FPGA2[128];  //spi.c
extern uint8_t ALLSTART;  //main
extern uint8_t SHOWSTART;
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)==1)
	{
		delay_ms(10);
		if(K_RIGHT==0)
		{
////			beep=0;
//			//SPIADC
////			SPI_Read_Fpga(0x00,SPI_FPGA1,128);
////			delay_ms(10);
////			SPI_Read_Fpga(0x00,SPI_FPGA1,128);
//			Add_SPI(SPI_FPGA1,SPI_FPGA2,128*2);
//			delay_ms(5);
//			Add_SPI(SPI_FPGA1,SPI_FPGA2,128*2);
//			
//			Deal_Data(SPI_FPGA1,RESULT1,SIZE);
//			LCD_Fill(20,200,480,800,WHITE);
////			Draw_Coordinate(50,600,400,300,1);
//			Draw_Coordinate(50,625,400,450,1);
//			Draw_Coordinate(50,420,400,270,0);
//			LCD_DL_F(50,625,RESULT1,RESULT1,SIZE,BLACK);
////			//fft
//			CalXiebo(RESULT1);
////			//ڷ
////			Usart_Mes(KINDA,FREA,KINDB,FREB,0);
//			if(ALLSTART==1)
//			{
////				Show_Wavemessage(KINDA,KINDB,FREA,FREB);
//				//ʾ
//	//			Draw_Wave(KINDA,KINDB,FREA,FREB,128);
//				
//			}
//			Usart_Mes(KINDA,FREA,KINDB,FREB,WAVEPHASE);
			if(ALLSTART==0) ALLSTART=1;
			SHOWSTART=1;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/*******************************************************************************
*            : EXTI4_IRQHandler
* 		   : ⲿж4
*              : 
*              : 
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)
	{
		delay_ms(10);
		if(K_LEFT==0)
		{
//			beep=1;
			ADDMODE=((ADDMODE==3)?0:ADDMODE+1);
			if(ADDMODE==0) LCD_ShowNum(50,20,5,2,16);
			else if(ADDMODE==1) LCD_ShowNum(50,20,10,2,16);
			else if(ADDMODE==2) LCD_ShowNum(50,20,20,2,16);
			else if(ADDMODE==3) LCD_ShowNum(50,20,1,2,16);
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void Show_Wavemessage(uint8_t kind_a,uint8_t kind_b,uint8_t fre_a,uint8_t fre_b)
{
//	LCD_ShowChar(230,29,'A',24,0);
//	LCD_ShowChar(242,29,':',24,0);
//	LCD_Fill(20,400,470,800,0xffff);
	if(kind_a==0x17)
	{
		LCD_ShowFontHZ(254, 25,"ǲ");
	}
	else LCD_ShowFontHZ(254, 25,"Ҳ");
	LCD_ShowNum(350,29,fre_a,3,24);
	LCD_ShowString(386,29,12*3,24,24,"kHz");
	
//	LCD_ShowChar(230,59,'B',24,0);
//	LCD_ShowChar(242,59,':',24,0);
	if(kind_b==0x17)
	{
		LCD_ShowFontHZ(254, 55,"ǲ");
	}
	else LCD_ShowFontHZ(254, 55,"Ҳ");
	LCD_ShowNum(350,59,fre_b,3,24);
	LCD_ShowString(386,59,12*3,24,24,"kHz");
}

//extern float FDATA_X[SIZE];
void Draw_Wave(uint8_t kind_a,uint8_t kind_b,uint8_t fre_a,uint8_t fre_b,uint8_t size)
{
	static float wavea[128],waveb[128];
	float fre=fre_b*1.0/fre_a;
	if(kind_a==1)
	{
		Creat_TriWave(1,1.0,0,size,wavea);
	}
	else Creat_Wave(1,1,0,size,wavea);
	Draw_Coordinate(50,600,400,300,1);
	LCD_DL_F(50,600,wavea,wavea,size,BLUE);
	
	if(kind_b==1)
	{
		Creat_TriWave(1,fre,0,size,waveb);
	}
	else Creat_Wave(1,fre,0,size,waveb);
	LCD_DL_F(50,600,wavea,waveb,size,RED);
	
}


#include "ffttest.h"
#include "data.h"
#include "system.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "tftlcd.h"
#include "my_lcd.h"

extern float32_t data1handle[SAMPLE_N * 2];
extern float32_t data2handle[SAMPLE_N * 2];
extern float32_t FFT_Output1[SAMPLE_N];
extern float32_t FFT_Output2[SAMPLE_N];

extern float32_t xiebo[SAMPLE_N];
extern float32_t phaseA[SAMPLE_N];
extern float32_t phaseB[SAMPLE_N];
extern float32_t phaseC[SAMPLE_N];

void Create_data1handle(float32_t *p)
{
    for(int i = 0;i < SAMPLE_N; i++)
    {
        data1handle[2 * i]     = p[i]*20;  //5
        data1handle[2 * i + 1] = 0;
    }
}

void Create_data2handle(float32_t *p)
{
    for(int i = 0;i < SAMPLE_N; i++)
    {
        data2handle[2 * i]     = p[i]*20;  //5
        data2handle[2 * i + 1] = 0;
    }
}

uint8_t FREA,FREB,KINDA,KINDB;
uint32_t index1,index2;
float PHASE_1,PHASE_2;
float Phase;
float CalXiebo(float32_t *wave)
{
	float max1,max2,Atop,Btop;
	float cisan=19.99,jiaosan=274.9;
	
	
    Create_data1handle(wave);
    arm_cfft_f32(&arm_cfft_sR_f32_len128, data1handle, 0, 1);   //һΪ  ڶΪ
    arm_cmplx_mag_f32(data1handle, FFT_Output1, SAMPLE_N);      //һһҶ任ֽ  ڶƵףģֵ
															//תģ
//	uint16_t pointy_s,pointy_e;
//	for(uint16_t i=0,j=0;i<SAMPLE_N/2;i++,j++)
//	{
//		pointy_s=FFT_Output1[i-1]/5;
//		pointy_e=FFT_Output1[i]/5;
//		if(i!=0) LCD_DrawLine_Color(50+i*5,420-pointy_s,50+i*5+5,420-pointy_e,BLACK);
//		LCD_DrawLine_Color(50+i*5,420,50+i*5,420-10,RED);
//	}
	
	arm_max_f32(FFT_Output1,SAMPLE_N,&max1,&index1);
	if(index1>0x003f) index1=SAMPLE_N-index1;
	FFT_Output1[index1]=0;FFT_Output1[SAMPLE_N-index1]=0;
	arm_max_f32(FFT_Output1,SAMPLE_N,&max2,&index2);
	if(index2>0x003f) index2=SAMPLE_N-index2;
	
	if(index1>index2)
	{
		FREA=index2;FREB=index1;  //г˷ֱ
		Atop=max2;Btop=max1;
	}
	else
	{
		FREA=index1;FREB=index2;
		Atop=max1;Btop=max2;
	}
	
	Phase=Calphase_low(data1handle,FREA);
	
	if((FREA==1||FREA==2||FREA==3||FREA==4||FREA==5||FREA==6)&&(Atop<275))
	{
		if(FREA==1&&FREB==3)
		{
			KINDB=(FFT_Output1[9]>cisan)?0x17:0x13;
			KINDA=0x17;
		}
		else if(FREA==2&&FREB==6)
		{
			KINDB=(FFT_Output1[18]>cisan)?0x17:0x13;
			KINDA=0x17;
		}
		else if(FREA==3&&FREB==9)
		{
			KINDB=(FFT_Output1[27]>cisan)?0x17:0x13;
			KINDA=0x17;
		}
		else if(FREA==4&&FREB==12)
		{
			KINDB=(FFT_Output1[36]>cisan)?0x17:0x13;
			KINDA=0x17;
		}
		else if(FREA==5&&FREB==15)
		{
			KINDB=(FFT_Output1[15*3]>cisan)?0x17:0x13;
			KINDA=0x17;
		}
		else if(FREA==6&&FREB==18)
		{
			KINDB=(FFT_Output1[18*3]>cisan)?0x17:0x13;
			KINDA=0x17;
		}
		else
		{
			KINDB=((Btop<jiaosan)?0x17:0x13);
			KINDA=0x17;
		}
	}
	else
	{
		KINDA=((Atop<jiaosan)?0x17:0x13);
		KINDB=((Btop<jiaosan)?0x17:0x13);
	}
	FREA*=5;FREB*=5;
	return Phase;
}

float CalShuchu1(float32_t *wave)  //ź
{
	float phase,Real,Imaginary;
	float hudu,angel,jiaodu;
	float max_val;
	uint32_t index;
	Create_data1handle(wave);  //һ
    arm_cfft_f32(&arm_cfft_sR_f32_len128, data1handle, 0, 1); //Ҷ
    arm_cmplx_mag_f32(data1handle, FFT_Output1, SAMPLE_N);  //Ƶ
	arm_max_f32(FFT_Output1,SAMPLE_N,&max_val,&index);
	
	if(index>0x003f) index=SAMPLE_N-index;
	Real     = data1handle[index * 2];
    Imaginary = data1handle[index * 2 + 1];
	hudu = atan2(Imaginary , Real);
	angel = hudu * 180.0 / 3.1415926;
	
	phase=angel+PI;
	return phase;
}

float CalShuchu2(float32_t *wave)  //ź
{
	float phase,Real,Imaginary;
	float hudu,angel,jiaodu;
	float max_val;
	uint32_t index;
	Create_data2handle(wave);  //һ
    arm_cfft_f32(&arm_cfft_sR_f32_len128, data2handle, 0, 1); //Ҷ
    arm_cmplx_mag_f32(data2handle, FFT_Output2, SAMPLE_N);  //Ƶ
	arm_max_f32(FFT_Output2,SAMPLE_N,&max_val,&index);
	
	if(index>0x003f) index=SAMPLE_N-index;
	Real     = data2handle[index * 2];
    Imaginary = data2handle[index * 2 + 1];
	hudu = atan2(Imaginary , Real);
	angel = hudu * 180.0 / 3.1415926;

	phase=angel+PI;
	return phase;
}





float Calphase_low(float *data,uint8_t indexa)
{
	float Reala,Imaginarya;
	float hudua,jiaodua,angel_ma;
	float phase;
	Reala     = data[indexa * 2];
    Imaginarya = data[indexa * 2 + 1];
//	hudua = atan2(Imaginarya , Reala);
	hudua = atan2(Imaginarya , Reala);
	angel_ma = hudua * 180.0 / 3.1415926;
	jiaodua=angel_ma;
//	if(indexa<SAMPLE_N/2)
//	{
////		arm_abs_f32(&angel_ma,&jiaodua,1);
//		jiaodua=angel_ma+90;
//		if(jiaodua>180) jiaodua-=180;
//		else if(jiaodua<0) jiaodua+=180;
//	}
//	else
//	{
////		arm_abs_f32(&angel_ma,&jiaodua,1);
//		jiaodua+=90;
//		if(jiaodua>180) jiaodua-=180;
//		else if(jiaodua<0) jiaodua+=180;
//	}
	phase=jiaodua+PI;
	return phase;
}

//float32_t jiaodu;
//uint32_t Index;
//float32_t CalPhase_m(float32_t *phase)
//{
////    int Index_50Hz = 50 / FFT_DPI;  //50Hz±꣬50/ֱ  ֱ=3200/64
//	
//	float32_t max_val;
//	float32_t Real,Imaginary,hudu,angel_m;//jiaodu;
//    
//    Create_data2handle(phase);  //һ
//    arm_cfft_f32(&arm_cfft_sR_f32_len128, data2handle, 0, 1); //Ҷ
//    arm_cmplx_mag_f32(data2handle, FFT_Output1, SAMPLE_N);  //Ƶ
//	arm_max_f32(FFT_Output1,SAMPLE_N,&max_val,&Index);
////	location=*Index;
//    Real      = data2handle[Index * 2];
//    Imaginary = data2handle[Index * 2 + 1];
//	hudu = atan2(Imaginary , Real);
//		
//	angel_m = hudu * 180.0 / 3.1415926;
//	if(Index<SAMPLE_N/2)
//	{
//		jiaodu=angel_m+90;
//	}
//	else
//	{
//		arm_abs_f32(&angel_m,&jiaodu,1);
//		jiaodu+=90;
//		if(jiaodu>180) jiaodu-=180;
//	}
////    
////	jiaodu=(angel_m>90?180-angel_m:angel_m);
////	angel_m = hudu * 180.0 / 3.1415926;
////	jiaodu=angel_m;  //-90~90ĳ0~180
//	return angel_m;
//}
//void CalPhase(float32_t *phase,char phase_name)
//{
//    int Index_50Hz = 50 / FFT_DPI;  //50Hz±꣬50/ֱ  ֱ=3200/64
//    float32_t Real,Imaginary,hudu;//jiaodu;
//    Create_data2handle(phase);  //һ
//    arm_cfft_f32(&arm_cfft_sR_f32_len128, data2handle, 0, 1); //Ҷ
//    arm_cmplx_mag_f32(data2handle, FFT_Output, SAMPLE_N);  //Ƶ

//    Real      = data2handle[Index_50Hz * 2];
//    Imaginary = data2handle[Index_50Hz * 2 + 1];

//    hudu = atan2(Imaginary , Real);
//    jiaodu = hudu * 180.0 / 3.1415926;
////    App_SerPrintf("%cλ:%f\n",phase_name,jiaodu);
//}

//void FFTtest(void)
//{
//    CalXiebo(xiebo);
//    CalPhase(phaseA,'A');
//    CalPhase(phaseB,'B');
//    CalPhase(phaseC,'C');
//}

//float angel[3];
//void Cal_WaveAPhase(float *wave,float *wa,float *wb,float *wc,u16 size)
//{
//	int Index_50Hz = 50 / FFT_DPI;  //50Hz±꣬50/ֱ  ֱ=3200/64
//    float32_t Real,Imaginary,hudu;//jiaodu;
//	
//	Create_data2handle(wave);
//    arm_cfft_f32(&arm_cfft_sR_f32_len64, data2handle, 0, 1);   //һΪ  ڶΪ
//    arm_cmplx_mag_f32(data2handle, FFT_Output, size);      //һһҶ任ֽ  ڶƵףģֵ
//															//תģ
//	uint16_t pointy_s,pointy_e;
//	for(uint16_t i=0,j=0;i<SAMPLE_N/2;i++,j++)
//	{
//		pointy_s=FFT_Output[i-1];
//		pointy_e=FFT_Output[i];
//		LCD_DrawLine_Color(50+i*5,400-pointy_s,50+i*5+5,400-pointy_e,BLACK);
//		LCD_DrawLine_Color(50+i*5,400,50+i*5,400-10,RED);
//	}  
//	
//	Create_data2handle(wa);  //һ
//    arm_cfft_f32(&arm_cfft_sR_f32_len64, data2handle, 0, 1); //Ҷ
//	Real      = data2handle[Index_50Hz * 2];
//    Imaginary = data2handle[Index_50Hz * 2 + 1];
//    hudu = atan2(Imaginary , Real);
//    angel[0] = hudu * 180.0 / 3.1415926;
//	
//	Create_data2handle(wb);  //һ
//    arm_cfft_f32(&arm_cfft_sR_f32_len64, data2handle, 0, 1); //Ҷ
//	Real      = data2handle[Index_50Hz * 2];
//    Imaginary = data2handle[Index_50Hz * 2 + 1];
//    hudu = atan2(Imaginary , Real);
//    angel[1] = hudu * 180.0 / 3.1415926;
//	
//	Create_data2handle(wc);  //һ
//    arm_cfft_f32(&arm_cfft_sR_f32_len64, data2handle, 0, 1); //Ҷ
//	Real      = data2handle[Index_50Hz * 2];
//    Imaginary = data2handle[Index_50Hz * 2 + 1];
//    hudu = atan2(Imaginary , Real);
//    angel[1] = hudu * 180.0 / 3.1415926;
//}


#include "myFFT.h"
#include "table_fft.h"
#include "stm32_dsp.h"
#include "WS2812.h"

uint8_t cardinal_number = 10;

int32_t fft_outBUF[NPT];
int32_t lBUFMAG[NPT + NPT/2];/* Magnitude vector */	

int32_t lBufMagArray[NPT];
int32_t fft_outBUF[NPT];
extern int32_t ADC_inBUF[NPT];
extern int8_t adc_transfer_flag;
/******************************************************************
函数名称:GetPowerMag()
函数功能:计算各次谐波幅值
参数说明:
备　　注:先将lBufOutArray分解成实部(X)和虚部(Y)，然后计算幅值(sqrt(X*X+Y*Y)
作　　者:博客园 依旧淡然（http://www.cnblogs.com/menlsh/）
*******************************************************************/
void GetPowerMag()
{
    int16_t lX,lY;
    float X,Y,Mag;
    for(uint16_t i = 0; i < NPT / 2; i++)
    {
			lX  = (fft_outBUF[i] << 16) >> 16;
			lY  = (fft_outBUF[i] >> 16);
			X = NPT * ((float)lX) / 32768;
			Y = NPT * ((float)lY) / 32768;
			Mag = (sqrt(X * X + Y * Y) * 1.0) / NPT;
			if(i == 0)
				lBufMagArray[i] = (uint16_t)(Mag * 32768);
			else{
				lBufMagArray[i] = (uint16_t)(Mag * 65536);
				if(lBufMagArray[i] > cardinal_number){
					lBufMagArray[i] = lBufMagArray[i] - cardinal_number;
					printf("\r\n ---lBufMagArray = %d",lBufMagArray[i]);
				}
				else{
					lBufMagArray[i] = 0;
				}
			}
    }
}
uint8_t G = 0, R = 0, B = 0;
extern uint8_t time;
void colorful(void){
	
	if(adc_transfer_flag == 1){
		adc_transfer_flag = 0;
		adc_transfer();
		uint32_t voice = 0;
		for(uint8_t i = 0; i < NPT; i++){
		voice = voice + ADC_inBUF[i];
		}
		
		voice = voice / NPT/45;
//
//		HAL_Delay(100);
		if(voice<49 && voice >41){
			return;
		}
		printf("\r\n-----voice%d----",voice);
		time = 0;
		if(voice > 90){
			voice = 90;
		}		
		if(voice < 10){
			voice = 10;
		}			
		voice = (voice -10)*2;
		if(voice > 90){
			voice = 90;
		}		
		if(voice > 45){
			for(uint8_t j = 45; j < voice; j++){
				WS2812_data12X8bit_data(j,255,255,0);	
				WS2812_send_data();
				HAL_Delay(20);
			}
		}		
		if(voice < 45){
			for(uint8_t j = 45; j > voice; j--){
				WS2812_data12X8bit_data(j,255,255,0);	
				WS2812_send_data();
				HAL_Delay(20);
			}
		}

//			HAL_Delay(500);
	//		WS2812_send_data();
			WS2812_Init();
		}
		star_adc_dma();
	
}
void FFT_calc(){
		if(adc_transfer_flag == 1){
			adc_transfer_flag = 0;
//			stop_adc_dma();
			adc_transfer();
			cr4_fft_64_stm32(fft_outBUF, ADC_inBUF, NPT);
			GetPowerMag();
			star_adc_dma();
//			colorful(lBufMagArray);
//			WS2812_send_data();
		}	
		
		
}

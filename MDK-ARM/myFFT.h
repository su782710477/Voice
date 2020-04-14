#ifndef __MYFFT_H
#define __MYFFT_H

//#define PI2 6.28318530717959


#include "main.h"
#include "math.h"
void	star_adc_dma(void);
void	WS2812_Init(void);
void	WS2812_send_data(void);
void adc_transfer(void);
void stop_adc_dma(void);
void star_adc_dma(void);
#define NPT 3            /* NPT = No of FFT point*/
void GetPowerMag(void);
void FFT_calc(void);
void colorful(void);
#endif

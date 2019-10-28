#include "WS2812.h"

//8bit color mapping 32bit color
//1 = 1110;	0 = 1000;
uint32_t WS2812_color[3] = {0x00};
//array initialization 0x00
//2 dimensional array; Front_rest_code row is rest code, @para at least 80us low level 
//12 column is 72bit(24bit color mapping)
static uint8_t WS2812_data[LED_number + Front_rest_code + Behind_rest_code][12] = {0x00}; 
//2 dimensional array convert 1 dimensional																		
static uint8_t WS2812_data_1D[(LED_number + Front_rest_code + Behind_rest_code) * 12] = {0x00};

/**
*	@para mode: 1: sk6812 data format ---_ /-___ 4bit; 2: ws2812 data format -__ /--_ 3bit;
*	@para color: 255 bit color data;
* @return 32 bit data,4 bit spi data instead of 1bit ws2812 data, total 32bit(8*4)
*/
uint32_t WS2812_ledcolor_2_SPIbit(uint8_t mode, uint8_t color){
	uint32_t data = 0;
	if(mode == 0x01){
		for(int i = 0; i < 8; i++){
			if((color >> i) & 0x01){
				data = data | (0x0E << (4 * i));
			}
			else{
				data = data | (0x08 << (4 * i));
			}
		}
	}
	if(mode == 0x02){
			for(int i = 0; i < 8; i++){
			if((color >> i) & 0x01){
				data = data | (0x06 << (3 * i));
			}
			else{
				data = data | (0x04 << (3 * i));
			}
		}
	}
//	printf("---%08x", data);
	return data;
}
/**
* @brif initialization ws2812/sk6812 data color(0,0,0)
*/
void WS2812_Init(){
	WS2812_color[LED_GREEN] = WS2812_ledcolor_2_SPIbit(SK6812, Single_Color_Black);		//green
	WS2812_color[LED_RED	] = WS2812_ledcolor_2_SPIbit(SK6812, Single_Color_Black);		//red 
	WS2812_color[LED_BLUE	] = WS2812_ledcolor_2_SPIbit(SK6812, Single_Color_Black);		//blue
	printf("\r\nws2812 init");		
	uint8_t WS2812_SPIbit32X3 = 0;		//
//	printf("\r\nsend data");
	for(int r = 0; r < LED_number + Front_rest_code + Behind_rest_code; r++){
		for(int c = 0; c < 12; c++) 
		{
			if(((c % 4) == 0)&&(c != 0)){
				WS2812_SPIbit32X3++;
			}
			WS2812_data[r][c] = (WS2812_color[c % 3] >> ((3 -(c % 4)) * 8));
		}
	}
}
/**
*color format	8bit -> 24bit
*						R:0x** -> 0x00(abandon) 0x** 0x** 0x**
*						G:0x** -> 0x00(abandon) 0x** 0x** 0x**
*						B:0x** -> 0x00(abandon) 0x** 0x** 0x**
*								total:72bit(9byte)
*
*/

void WS2812_data9X8bit_data(uint8_t number, uint8_t G, uint8_t R, uint8_t B){
	WS2812_color[LED_GREEN] = WS2812_ledcolor_2_SPIbit(WS2812,G);
	WS2812_color[LED_RED	] = WS2812_ledcolor_2_SPIbit(WS2812,R);
	WS2812_color[LED_BLUE	] = WS2812_ledcolor_2_SPIbit(WS2812,B);
	
	uint8_t WS2812_SPIbit24X3 = 0;
//	printf("\r\nsend data");
	for(int i = 0; i < 9; i++) 
	{
		if(((i % 3) == 0)&&(i != 0)){
			WS2812_SPIbit24X3++;
		}
		WS2812_data[number][i] = (WS2812_color[WS2812_SPIbit24X3] >> ((2 -(i % 3)) * 8));
//		printf("9X8bit->0x%02x", WS2812_data[number][i]);
	}
}
/**
* depend on the code format -__ /--_ 3bit or ---_ /-___ 4bit
*color format	8bit -> 32bit
*						R:0x** -> 0x00(abandon) 0x** 0x** 0x** 0x**
*						G:0x** -> 0x00(abandon) 0x** 0x** 0x** 0x**
*						B:0x** -> 0x00(abandon) 0x** 0x** 0x** 0x**
*							 total: 96bit(12byte)
*
*/
void WS2812_data12X8bit_data(uint8_t number, uint8_t G, uint8_t R, uint8_t B){
	WS2812_color[LED_GREEN] = WS2812_ledcolor_2_SPIbit(SK6812,G);
	WS2812_color[LED_RED	] = WS2812_ledcolor_2_SPIbit(SK6812,R);
	WS2812_color[LED_BLUE	] = WS2812_ledcolor_2_SPIbit(SK6812,B);
	
	uint8_t WS2812_SPIbit24X3 = 0;
//	printf("\r\nsend data");
	for(int i = 0; i < 12; i++) 
	{
		if(((i % 4) == 0)&&(i != 0)){
			WS2812_SPIbit24X3++;
		}
		WS2812_data[number][i] = (WS2812_color[WS2812_SPIbit24X3] >> ((3 -(i % 4)) * 8));
//		printf("9X8bit->0x%02x", WS2812_data[number][i]);
	}
}

void arry_2Dto1D(){
	for(int i = 0; i < Front_rest_code; i++){	//	row
		for(int j = 0; j<12; j++){		//column
			WS2812_data_1D[i * 12 + j] = 0x00;
		}
	}
	for(int i = Front_rest_code; i < LED_number + Front_rest_code; i++){	//	row
		for(int j = 0; j < 12; j++){		//column
			WS2812_data_1D[i * 12 + j] = WS2812_data[i-Front_rest_code][j];
		}
	}
	for(int i = LED_number + Front_rest_code; i < LED_number + Front_rest_code + Behind_rest_code; i++){	//	row
		for(int j = 0; j<12; j++){		//column
			WS2812_data_1D[i * 12 + j] = 0x00;
		}
	}
}
void WS2812_send_data(){

	arry_2Dto1D();
//	printf("\r\nsend data\r\n");
//	for(int i = 0; i < (WS2812_number + Front_rest_code + Behind_rest_code) * 12; i++){
//		printf(" %02x", WS2812_data_1D[i]);
//		if((i+1) % 12 == 0){
//		printf("\r\n-------------------------------------\r\n");
//		}
//	}
	HAL_SPI_Transmit(&hspi1, WS2812_data_1D, (LED_number + Front_rest_code + Behind_rest_code) * 12, 0xffffffff);
}




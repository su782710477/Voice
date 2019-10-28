#ifndef __WS2812_H
#define __WS2812_H

#include "main.h"
#include "spi.h"
#define LED_number 30
#define Front_rest_code 3
#define Behind_rest_code 1
#define SK6812 1
#define WS2812 2

#define LED_GREEN 0
#define LED_RED 1
#define LED_BLUE 2

#define Single_Color_Black 0

void WS2812_data9X8bit_data(uint8_t number, uint8_t G, uint8_t R, uint8_t B);
void WS2812_data12X8bit_data(uint8_t number, uint8_t G, uint8_t R, uint8_t B);
void WS2812_send_data();
void WS2812_Init();
#endif
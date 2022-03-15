#include <stdint.h>
#include <stdio.h>

#define PIXEL_BUFFER_BASE_ADDRESS (volatile uint16_t *) 0x08000000

#define WHITE_COLOR	0xFFFF
#define BLACK_COLOR 0x0000
#define PURPLE_COLOR 0xF81F
#define CHARTREUSE_COLOR 0x7FE0

void drawLine (int16_t x0, int16_t x1, uint16_t y0, int16_t y1);
void drawPixelColor(int16_t x, int16_t y, uint16_t color);
void drawPixel(int16_t x, int16_t y);
uint32_t offsetCalculator(uint16_t x, uint16_t y);

int main (void)
{
	uint16_t x = 0;
	uint16_t y = 0;
	for(x = 0; x < 320; x++) {
		for(y = 0; y < 240; y++) {
			drawPixel(x, y);
		}
	}

	/*
	//Draw white pixels
	for(y = 0; y < 240; y++) {
		for(x = 0; x < 320; x++) {
			*(PIXEL_BUFFER_BASE_ADDRESS + (y<<9) + x) = 0xFFFF;
		}
	}
	*/
	drawLine(0,319,0,239);
	drawLine(319,0,0,239);

	return 0;
}

void drawLine (int16_t x0, int16_t x1, uint16_t y0, int16_t y1)
{
	int16_t buf = 0;
	int8_t isSteep = abs(y1 - y0) > abs(x1 - x0);
	if (isSteep) {
		buf = x0;
		x0 = y0;
		y0 = buf;

		buf = x1;
		x1 = y1;
		y1 = buf;
	}
	if(x0 > x1) {
		buf = x0;
		x0 = x1;
		x1 = buf;

		buf = y0;
		y0 = y1;
		y1 = buf;
	}

	int16_t deltax = x1 - x0;
	int16_t deltay = abs(y1-y0);
	int16_t error = -(deltax /2);
	int16_t y = y0;
	int8_t yStep = 0;

	if(y0 < y1) {
		yStep = 1;
	} else {
		yStep = -1;
	}

	int16_t x = 0;
	for(x = x0; x < x1; x++) {
		if(isSteep) {
			drawPixelColor(y,x, CHARTREUSE_COLOR);
		} else {
			drawPixelColor(x,y, CHARTREUSE_COLOR);
		}
		
		error = error + deltay;
		
		if(error >= 0) {
			y = y + yStep;
			error = error - deltax;
		}
	}


}

void drawPixelColor(int16_t x, int16_t y, uint16_t color)
{
	*(PIXEL_BUFFER_BASE_ADDRESS + offsetCalculator(x,y)) = color;
}

void drawPixel(int16_t x, int16_t y)
{
	*(PIXEL_BUFFER_BASE_ADDRESS + offsetCalculator(x,y)) = WHITE_COLOR;
}

uint32_t offsetCalculator(uint16_t x, uint16_t y)
{
	return (y<<9) + x;
}

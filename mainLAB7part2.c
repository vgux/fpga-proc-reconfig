#include <stdint.h>
#include <stdio.h>

#define PIXEL_BUFFER_BASE_ADDRESS 	(volatile uint16_t *) 	0x08000000
#define PIXEL_STATUS_REGISTER		(volatile uint32_t *) 	0x1000302C
#define BUFFER_REGISTER 			(volatile uint32_t *)	0x10003020

#define WHITE_COLOR	0xFFFF
#define BLACK_COLOR 0x0000
#define PURPLE_COLOR 0xF81F
#define CHARTREUSE_COLOR 0x7FE0
#define BLUE_COLOR		0x11F0

#define X_MAX	320
#define Y_MAX	240
#define LINE_LENGTH	10



void drawLine (int16_t x0, int16_t x1, uint16_t y0, int16_t y1);
void drawPixelColor(int16_t x, int16_t y, uint16_t color);
void drawPixel(int16_t x, int16_t y);
uint32_t offsetCalculator(uint16_t x, uint16_t y);

int main (void)
{
	uint16_t x = 0;
	uint16_t y = 0;
	for(x = 0; x < X_MAX; x++) {
		for(y = 0; y < Y_MAX; y++) {
			drawPixelColor(x, y, BLACK_COLOR);
		}
	}

	uint16_t pos = 1;
	int8_t dir = 1;

	while(1) {
		*BUFFER_REGISTER = 1;

		drawLineColor(15, 15, pos, pos + LINE_LENGTH, BLACK_COLOR);
		drawLineColor(16, 16, pos, pos + LINE_LENGTH, BLACK_COLOR);
		drawLineColor(17, 17, pos, pos + LINE_LENGTH, BLACK_COLOR);
		drawLineColor(18, 18, pos, pos + LINE_LENGTH, BLACK_COLOR);
		pos+=dir;
		drawLineColor(15, 15, pos, pos + LINE_LENGTH, WHITE_COLOR);
		drawLineColor(16, 16, pos, pos + LINE_LENGTH, BLUE_COLOR);
		drawLineColor(17, 17, pos, pos + LINE_LENGTH, CHARTREUSE_COLOR);
		drawLineColor(18, 18, pos, pos + LINE_LENGTH, PURPLE_COLOR);

		if((pos + LINE_LENGTH) == Y_MAX) {
			dir = -dir;
		} else if(pos == 0) {
			dir = -dir;
		}
		
		while((*PIXEL_STATUS_REGISTER) & 1 != 0);
	}


	return 0;
}

void drawLineColor (int16_t x0, int16_t x1, uint16_t y0, int16_t y1, uint16_t color)
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
			drawPixelColor(y,x, color);
		} else {
			drawPixelColor(x,y, color);
		}
		
		error = error + deltay;
		
		if(error >= 0) {
			y = y + yStep;
			error = error - deltax;
		}
	}
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

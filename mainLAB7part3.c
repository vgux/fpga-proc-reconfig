#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


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
#define LINE_NUMBER 4


void drawLine (int16_t x0, int16_t x1, int16_t y0, int16_t y1);
void drawPixelColor(int16_t x, int16_t y, uint16_t color);
void drawPixel(int16_t x, int16_t y);
uint32_t offsetCalculator(uint16_t x, uint16_t y);

int main (void)
{
	//Setting foreground to Black
	uint16_t x = 0;
	uint16_t y = 0;
	for(x = 0; x < X_MAX; x++) {
		for(y = 0; y < Y_MAX; y++) {
			drawPixelColor(x, y, BLACK_COLOR);
		}
	}

	//Initializing X and Y positions for lines
	uint16_t posX[LINE_NUMBER] = {0};
	uint16_t posY[LINE_NUMBER] = {0};
	int8_t dirLinesX[LINE_NUMBER] = {1, -1, -1, 1};
	int8_t dirLinesY[LINE_NUMBER] = {-1, -1, -1, -1};

	srand(NULL);
	uint8_t i = 0;

	for(i = 0; i<LINE_NUMBER; i++) {
		posX[i] = rand() % X_MAX;
		posY[i] = rand() % Y_MAX;
		dirLinesX[i] = (rand() % 2) -1;
		dirLinesY[i] = (rand() % 2) -1;
	}

	while(1) {
		*BUFFER_REGISTER = 1;

		drawLineColor(posX[0], posX[1], posY[0], posY[1], BLACK_COLOR);
		drawLineColor(posX[1], posX[2], posY[1], posY[2], BLACK_COLOR);
		drawLineColor(posX[2], posX[3], posY[2], posY[3], BLACK_COLOR);
		drawLineColor(posX[3], posX[0], posY[3], posY[0], BLACK_COLOR);

		for(i = 0; i<LINE_NUMBER; i++) {
			posX[i] += dirLinesX[i];
			posY[i] += dirLinesY[i];

			if(posX[i] > X_MAX) {
				posX[i] = X_MAX-1;
			} else if (posX[i] < 0) {
				posX[i] = 0;
			}

			if(posY[i] > Y_MAX) {
				posY[i] = Y_MAX-1;
			} else if (posY[i] < 0) {
				posY[i] = 0;
			}
		}

		drawLineColor(posX[0], posX[1], posY[0], posY[1], WHITE_COLOR);
		drawLineColor(posX[1], posX[2], posY[1], posY[2], BLUE_COLOR);
		drawLineColor(posX[2], posX[3], posY[2], posY[3], CHARTREUSE_COLOR);
		drawLineColor(posX[3], posX[0], posY[3], posY[0], PURPLE_COLOR);

		for(i = 0; i<LINE_NUMBER; i++) {
			if(posY[i] == Y_MAX) {
				dirLinesY[i] = (rand() % 2) -1;
			} else if(posY[i] == 0) {
				dirLinesY[i] = (rand() % 2) -1;
			}

			if(posX[i] == X_MAX) {
				dirLinesX[i] = (rand() % 2) -1;
			} else if(posY[i] == 0) {
				dirLinesX[i] = (rand() % 2) -1;
			}
		}
	
		while((*PIXEL_STATUS_REGISTER) & 1 != 0);
	}

	return 0;
}

void drawLineColor (int16_t x0, int16_t x1, int16_t y0, int16_t y1, uint16_t color)
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

void drawLine (int16_t x0, int16_t x1, int16_t y0, int16_t y1)
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

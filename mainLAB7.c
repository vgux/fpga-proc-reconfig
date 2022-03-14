#include <stdint.h>
#include <stdio.h>

void drawLine (uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1);
void drawPixel(int16_t x, int16_t y);

int main (void)
{



}

void drawLine (int16_t x0, int16_t x1, uint16_t y0, int16_t y1)
{
	int16_t buf;
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
			drawPixel(y,x);
		} else {
			drawPixel(x,y);
		}
		
		error = error + deltay;
		
		if(error >= 0) {
			y = y + yStep;
			error = error - deltax;
		}
	}


}

void drawPixel(int16_t x, int16_t y)
{
	int a;
}

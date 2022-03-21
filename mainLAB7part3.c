#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>

#define PIXELMAP_BASE_ADRESS	(volatile uint16_t*)	0x08000000
#define STATUS_REGISTER 		(volatile uint16_t*)	0x1000302C
#define FRONT_BUFFER_REGISTER   (volatile uint16_t*)	0x10003020

#define X_MAX                  320 
#define Y_MAX                  240
#define BLACK_COLOR            0x0000
#define WHITE_COLOR            0xFFFF
#define RED_COLOR              0xF800
#define PURPLE_COLOR           0xF81F
#define GRAY_COLOR             0x8410
#define GREEN_COLOR            0x7E00
#define BLUE_COLOR             0x001F
#define COLOR_NUMBER           6
#define NODE_NUMBER             10


void drawPixelColor(uint16_t x, uint16_t y, uint16_t color);
void drawBackgroundColor(uint16_t color);
void drawLineColor(int x0, int x1, int y0, int y1, int color);

typedef struct {
	int xCoord;
	int yCoord;
	
	int xDir;
	int yDir;
} node_t;

int main(void)
{
	int index	= 0;
	int p_index	= Y_MAX-1;
	int node_index;
	int color_array[COLOR_NUMBER] = {0xFFFF, 0xF800, 0xF81F, 0x8410, 0x7E00, 0x001F};

	node_t node_array[NODE_NUMBER];
	
	drawBackgroundColor(BLACK_COLOR);
	
	for(node_index = 0; node_index < NODE_NUMBER; node_index++)
	{
		node_array[node_index].xCoord = rand() % X_MAX;
		node_array[node_index].yCoord = rand() % Y_MAX;
	}
	
	while(1)
	{
		// Wait next write cycle
		*FRONT_BUFFER_REGISTER = 1;
		while ((*STATUS_REGISTER & 1) !=0);
		
		
		// ERASE figure
		for(node_index = 0; node_index < NODE_NUMBER; node_index++)
		{
			drawLineColor(node_array[node_index].xCoord, 
					  node_array[(node_index + 1) % NODE_NUMBER].xCoord, 
					  node_array[node_index].yCoord, 
					  node_array[(node_index + 1) % NODE_NUMBER].yCoord, 
					  BLACK_COLOR);
		}
		
		// Incremente node coord
		for(node_index = 0; node_index < NODE_NUMBER; node_index++)
		{
			if (node_array[node_index].xDir == 1) {
				if(node_array[node_index].xCoord >= X_MAX) {
					node_array[node_index].xDir = 0;
				} else  {
					node_array[node_index].xCoord++;
				}
			} else {
				if(node_array[node_index].xCoord <= 0) {
					node_array[node_index].xDir = 1;
				} else {
					node_array[node_index].xCoord--;
				}
			}
			
			if (node_array[node_index].yDir == 1) {
				if(node_array[node_index].yCoord >= Y_MAX) {
					node_array[node_index].yDir = 0;
				} else {
					node_array[node_index].yCoord++;
				}
			} else {
				if(node_array[node_index].yCoord <= 0) {
					node_array[node_index].yDir = 1;
				} else {
					node_array[node_index].yCoord--;
				}
			}
		}
		
		// Draw figure
		for(node_index = 0; node_index < NODE_NUMBER; node_index++) {
			drawLineColor(node_array[node_index].xCoord, 
						  node_array[(node_index + 1) % NODE_NUMBER].xCoord, 
						  node_array[node_index].yCoord, 
						  node_array[(node_index + 1) % NODE_NUMBER].yCoord, 
						  PURPLE_COLOR);
		}
	}
}

void drawLineColor(int x0, int x1, int y0, int y1, int color)
{
	uint8_t isSteep;
	uint16_t x = 0, y  = 0, m_x1 = 0, m_y0 = 0;
	int16_t deltax = 0, deltay = 0, error = 0, y_step = 0;

	
	if (abs(y1 - y0) > abs(x1 - x0)) {
		isSteep = 1;
	} else {
		isSteep = 0;
	}
	
	if(isSteep) {
		// swap(x0, y0)
		m_y0= y0;
		y0	= x0;
		x0	= m_y0;
		
		//swap(x1, y1)
		m_x1= x1;
		x1	= y1;
		y1	= m_x1;
	}

	if(x0 > x1) {
		// swap(x0, x1)
		m_x1= x1;	
		x1	= x0;
		x0	= m_x1;
		
		// swap(y0, y1)
		m_y0= y0;
		y0	= y1;
		y1	= m_y0;
	}
	
	deltax = x1 - x0;
	deltay = abs(y1 - y0);
	error = -(deltax / 2);
	y = y0;
	
	if(y0 < y1) {
		y_step = 1;
	} else {
		y_step = -1;
	}
	
	for(x = x0; x < x1; x++) {
		if(isSteep) {
			drawPixelColor(y, x, color);
		} else {
			drawPixelColor(x, y, color);
		}

		error = error + deltay;

		if(error >= 0) {
			y = y + y_step;
			error = error - deltax;
		}
	}
}

void drawPixelColor(uint16_t x, uint16_t y, uint16_t color)
{
	*(PIXELMAP_BASE_ADRESS + ((y<<9) + x)) = color;
}

void drawBackgroundColor(uint16_t color)
{
	uint16_t x, y;
	
	for (x = 0; x < X_MAX; x++) {
		for (y = 0; y < Y_MAX; y++) {
			drawPixelColor(x, y, color);
		}
	}
}

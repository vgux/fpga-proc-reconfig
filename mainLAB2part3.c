#include <stdint.h>
#include <stdio.h>

#define switches (volatile char *) 0x10000040
#define ledR (volatile char *) 0x10000000
#define ledG (volatile char *) 0x10000010

#define pushbutton_edge_capture (volatile char *) 0x1000005C

#define HEX (volatile uint32_t *) 0x10000020

uint32_t sevenSegmentsDisplayConverter(uint32_t value);

int main (void)
{
	*ledR = 0;
	*ledG = 0;
	*pushbutton_edge_capture = 0;

	//*HEX = 0x3F3F3F3F;
	*HEX = sevenSegmentsDisplayConverter(0);

	while(1) {
		*ledG = *switches;
		if(*pushbutton_edge_capture & 2) {
			*ledR += *switches;

			*HEX = sevenSegmentsDisplayConverter(*ledR);

			*pushbutton_edge_capture = 0;
		}
	
	}
}

uint32_t sevenSegmentsDisplayConverter(uint32_t value)
{
	unsigned char table[16] = { 0x3f, 0x06, 0x5b, 0x4f,
								0x66, 0x6d, 0x7d, 0x07,
								0x7f, 0x67,0x77, 0x7c,
								0x39, 0x5e, 0x79, 0x71 };

    uint32_t hexDisplay = 0;

    hexDisplay += table[((value >> 12) & 0xF)] << 24;
	hexDisplay += table[((value >> 8) & 0xF)]  << 16;
	hexDisplay += table[((value >> 4) & 0xF)]  << 8;
	hexDisplay += table[((value) & 0xF)];

    return hexDisplay;
}

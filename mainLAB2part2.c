#include <stdint.h>
#include <stdio.h>

#define switches (volatile char *) 0x10000040
#define ledR (volatile char *) 0x10000000
#define ledG (volatile char *) 0x10000010

#define pushbutton_edge_capture (volatile char *) 0x1000005C

int main (void)
{
	*ledR = 0;
	*ledG = 0;
	*pushbutton_edge_capture = 0;
	while(1) {

		if(*pushbutton_edge_capture & 2) {
			*ledG = *switches;
			*ledR += *switches;

			*pushbutton_edge_capture = 0;
		}
	
	}
}

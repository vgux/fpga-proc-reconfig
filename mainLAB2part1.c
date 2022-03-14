#include <stdint.h>
#include <stdio.h>

#define switches (volatile char *) 0x10000040
#define ledR (volatile char *) 0x10000000
#define ledG (volatile char *) 0x10000010

int main (void)
{
	*ledR = 0;
	while(1) {
		*ledG = *switches;
		*ledR += *switches;
	}
}

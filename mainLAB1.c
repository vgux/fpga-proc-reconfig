//Main LAB1

#include <stdint.h>
#include <stdio.h>

#define N	7

int main(void)
{
	int32_t list[N] = {15,13,3,6,1,8,2};
	int8_t counter = N;
	int8_t largestNumberSoFar = list[N-1];


	for(counter = N-2; counter >= 0; counter--) {
		if(list[counter] > largestNumberSoFar) {
			largestNumberSoFar = list[counter];
		}
	}

	printf("largestNumberSoFar = %d", largestNumberSoFar);

	return 0;
}

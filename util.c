#include <stdio.h>
#include "util.h"

void hex_dump(uint8_t buf[], int len)
{
	int i;

	for (i = 1; i <= len; i++) {
		printf("%02x", buf[i - 1]);
		if (i % 32 == 0)
			printf("\n");
		else if (i % 4 == 0)
			printf(" ");
	}

	if ((i - 1) % 32 != 0)
		printf("\n");
	printf("\n");
}

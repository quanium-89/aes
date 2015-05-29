#ifndef _H_UTIL
#define _H_UTIL

#include <stdint.h>

#define CEIL(x, y)  (int)((x + (y - 1)) / (y))

#define ROTL32(n, m)  do { \
		asm ( \
			"roll %1, %0\n\t" \
			: "=r"(n) \
			: "I"(m), "0"(n) \
		); \
	} while (0)

void hex_dump(uint8_t buf[], int len);

#endif

#ifndef __BL_HELPERS_H
#define __BL_HELPERS_H

typedef unsigned char uint_8b;
typedef unsigned short uint_16b;
typedef unsigned int uint_32b;
typedef unsigned long long uint_64b;

// bool
#ifndef true
typedef _Bool bool;
#define true 1
#define false 0
#endif

bool memcmp(void *a, void *b, int n);

#endif // __BL_HELPERS_H
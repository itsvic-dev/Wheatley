#ifndef __BL_HELPERS_H
#define __BL_HELPERS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#ifndef NULL
#define NULL 0
#endif

// bool
#ifndef true
typedef _Bool bool;
#define true 1
#define false 0
#endif

bool memcmp(void *a, void *b, int n);

#endif // __BL_HELPERS_H

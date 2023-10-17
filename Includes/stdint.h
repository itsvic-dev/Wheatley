#ifndef _STDINT_H
#define _STDINT_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef __SIZE_TYPE__ size_t;
typedef uint64_t uintptr_t;
typedef int64_t intmax_t;

#ifndef NULL
#define NULL 0
#endif

// bool
#ifndef true
typedef _Bool bool;
#define true 1
#define false 0
#endif

#endif // _STDINT_H

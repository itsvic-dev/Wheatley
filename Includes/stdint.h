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
typedef int64_t intptr_t;
typedef uint64_t uintmax_t;
typedef int64_t intmax_t;

// stdatomic needs this
typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;
typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;

// probably wrong, but idc
typedef int8_t int_fast8_t;
typedef uint8_t uint_fast8_t;
typedef int64_t int_fast16_t;
typedef uint64_t uint_fast16_t;
typedef int64_t int_fast32_t;
typedef uint64_t uint_fast32_t;
typedef int64_t int_fast64_t;
typedef uint64_t uint_fast64_t;

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

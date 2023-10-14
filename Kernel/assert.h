#ifndef __KERNEL_ASSERT_H
#define __KERNEL_ASSERT_H

#include "panic.h"

#define assert(expr) if (!(expr)) panic("Assertion failed: " #expr)

#endif // __KERNEL_ASSERT_H

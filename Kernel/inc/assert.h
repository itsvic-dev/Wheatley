#ifndef __KERNEL_ASSERT_H
#define __KERNEL_ASSERT_H

#include "panic.h"

#define assert(expr) if (!(expr)) panic("Assertion failed: " #expr, 0)

#endif // __KERNEL_ASSERT_H

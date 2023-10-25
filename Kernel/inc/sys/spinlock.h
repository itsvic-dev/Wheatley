#ifndef __KERNEL_SYS_SPINLOCK_H
#define __KERNEL_SYS_SPINLOCK_H

#include <stdint.h>
#include <assert.h>
#include <stdatomic.h>

typedef struct {
    atomic_flag locked;
} spinlock_t;

#define SPINLOCK_INIT {ATOMIC_FLAG_INIT}

static inline void spinlock_wait_and_acquire(spinlock_t *lock) {
    while (atomic_flag_test_and_set_explicit(&lock->locked, memory_order_acquire))
        asm("pause");
}

static inline void spinlock_release(spinlock_t *lock) {
    atomic_flag_clear_explicit(&lock->locked, memory_order_release);
}

#endif // __KERNEL_SYS_SPINLOCK_H

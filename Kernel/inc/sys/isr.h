#ifndef __KERNEL_ISR_H
#define __KERNEL_ISR_H

#include "stdint.h"

// more stolen code from polaris lol
#define __PASTER(x, y) x##y
#define PASTER(x, y) __PASTER(x, y)

// the ONE macro makes functions like "void isr0(void)"
// the counter is summed on each macro call
#define ONE PASTER(void isr, __COUNTER__)(void)

// different granularities
#define TWO ONE; ONE
#define TEN TWO; TWO; TWO; TWO; TWO
#define TWENTY TEN; TEN
#define HUNDRED TWENTY; TWENTY; TWENTY; TWENTY; TWENTY
// now define 255 ISRs based on the previous granularities
#define DEFISR \
    HUNDRED;   \
    HUNDRED;   \
    TWENTY;    \
    TWENTY;    \
    TEN;       \
    TWO;       \
    TWO;       \
    TWO;       \

DEFISR;

typedef struct {
    // registers from isr_common_format
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    // other data from isr_common_format
    uint64_t isrNumber;

    // registers from ISR stack frame
    uint64_t errorCode;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} registers_t;

typedef void (*event_handler_t)(registers_t *);
void isr_write(void);
extern void isr_handle(registers_t *r);
void isr_register_handler(int n, void *handler);

#endif // __KERNEL_ISR_H

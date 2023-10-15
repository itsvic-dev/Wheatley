#ifndef __KERNEL_SYS_CPUID_H
#define __KERNEL_SYS_CPUID_H

#include <stdint.h>

extern char cpu_oem_id[13];

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
} cpuid_data_t;

cpuid_data_t cpuid(uint64_t leaf);
void get_cpuid_string(char *buf, cpuid_data_t *data);

#endif // __KERNEL_SYS_CPUID_H

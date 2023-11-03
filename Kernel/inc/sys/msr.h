#ifndef __KERNEL_SYS_MSR_H
#define __KERNEL_SYS_MSR_H

#include <stdint.h>

static inline uint64_t rdmsr(uint32_t msr) {
  uint32_t low;
  uint32_t high;
  asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
  return (uint64_t)high << 32 | (uint64_t)low;
}

static inline void wrmsr(uint32_t msr, uint64_t value) {
  uint32_t low = value & 0xFFFFFFFF;
  uint32_t high = value >> 32;
  asm volatile("wrmsr" : : "a"(low), "d"(high), "c"(msr));
}

#endif // __KERNEL_SYS_MSR_H

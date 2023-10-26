#ifndef __KERNEL_SYS_MMIO_H
#define __KERNEL_SYS_MMIO_H

#include <stdint.h>

#define BYTE_PTR(ptr) (*((uint8_t *)ptr))
#define WORD_PTR(ptr) (*((uint16_t *)ptr))
#define DWORD_PTR(ptr) (*((uint32_t *)ptr))

static inline void mmoutb(void *addr, uint8_t value) {
  asm volatile("mov %0, %1" : "=m"(BYTE_PTR(addr)) : "r"(value) : "memory");
}

static inline void mmoutw(void *addr, uint16_t value) {
  asm volatile("mov %0, %1" : "=m"(WORD_PTR(addr)) : "r"(value) : "memory");
}

static inline void mmoutd(void *addr, uint32_t value) {
  asm volatile("mov %0, %1" : "=m"(DWORD_PTR(addr)) : "r"(value) : "memory");
}

static inline uint8_t mminb(void *addr) {
  uint8_t ret;
  asm volatile("mov %0, %1" : "=r"(ret) : "m"(BYTE_PTR(addr)) : "memory");
  return ret;
}

static inline uint16_t mminw(void *addr) {
  uint16_t ret;
  asm volatile("mov %0, %1" : "=r"(ret) : "m"(WORD_PTR(addr)) : "memory");
  return ret;
}

static inline uint32_t mmind(void *addr) {
  uint32_t ret;
  asm volatile("mov %0, %1" : "=r"(ret) : "m"(DWORD_PTR(addr)) : "memory");
  return ret;
}

#endif // __KERNEL_SYS_MMIO_H

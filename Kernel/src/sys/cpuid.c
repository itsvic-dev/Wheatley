#include <mm/mm.h>
#include <sys/cpuid.h>

char cpu_oem_id[13];

cpuid_data_t cpuid(uint64_t leaf) {
  cpuid_data_t data;
  asm volatile("mov rax, %[leaf]\n" // move leaf to RAX
               "cpuid;"             // call cpuid
               "mov %0, rax\n"      // move registers to cpuid_data_t
               "mov %1, rbx\n"
               "mov %2, rcx\n"
               "mov %3, rdx\n"
               : "=r"(data.rax), "=r"(data.rbx), "=r"(data.rcx), "=r"(data.rdx)
               : [leaf] "r"(leaf)
               : "rax", "rbx", "rcx", "rdx", "memory");
  return data;
}

void get_cpuid_string(char *buf, cpuid_data_t *data) {
  uint32_t *str = (uint32_t *)buf;
  str[0] = data->rbx;
  str[1] = data->rdx;
  str[2] = data->rcx;
}

uint8_t getApicID() {
  cpuid_data_t data = cpuid(1);
  return (uint8_t)(data.rbx >> 24);
}

#include <sys/cpuid.h>
#include <mm/mm.h>

char cpu_oem_id[13];

cpuid_data_t cpuid(uint64_t leaf) {
    cpuid_data_t data;
    asm volatile(
        "mov %[leaf], %%rax;" // move leaf to RAX
        "cpuid;" // call cpuid
        "mov %%rax, %0;" // move registers to cpuid_data_t
        "mov %%rbx, %1;"
        "mov %%rcx, %2;"
        "mov %%rdx, %3;"
        : "=r"(data.rax), "=r"(data.rbx), "=r"(data.rcx), "=r"(data.rdx)
        : [leaf] "r"(leaf)
        : "%rax", "%rbx", "%rcx", "%rdx", "memory"
    );
    return data;
}

void get_cpuid_string(char *buf, cpuid_data_t *data) {
    uint32_t *str = (uint32_t *)buf;
    str[0] = data->rbx;
    str[1] = data->rdx;
    str[2] = data->rcx;
}

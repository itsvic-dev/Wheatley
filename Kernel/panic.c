#include "stdint.h"
#include "panic.h"
#include "printf.h"

#define READ_REGISTER(reg) asm volatile("mov %%" #reg ", %0" : "=q"(reg))

__attribute__((noreturn)) void panic(const char *reason) {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rsp;
    uint64_t rbp;

    uint64_t cr0;
    uint64_t cr3;
    uint64_t cr4;
    uint64_t cr8;

    // get register states
    READ_REGISTER(rax);
    READ_REGISTER(rbx);
    READ_REGISTER(rsi);
    READ_REGISTER(rdi);
    READ_REGISTER(rsp);
    READ_REGISTER(rbp);
    READ_REGISTER(cr0);
    READ_REGISTER(cr3);
    READ_REGISTER(cr4);
    READ_REGISTER(cr8);

    printf("\n*** PANIC: %s ***\n", reason);

    // print register states
    printf("\nRegisters:\n");
    printf("  RAX: 0x%016llx  RBX: 0x%016llx\n", rax, rbx);
    printf("  RSI: 0x%016llx  RDI: 0x%016llx\n", rsi, rdi);
    printf("  RSP: 0x%016llx  RBP: 0x%016llx\n", rsp, rbp);
    printf("  CR0: 0x%016llx  CR3: 0x%016llx\n", cr0, cr3);
    printf("  CR4: 0x%016llx  CR8: 0x%016llx\n", cr4, cr8);

    printf("\nBacktrace:\n  TODO\n");

    printf("\n*** Halting now, good night.\n");
    for (;;) asm("hlt");
}


// implement SSP using panic
#define STACK_CHK_GUARD 0x595e9fbd94fda766

uint64_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void) {
    panic("stack smashing detected");
}

#include "stdint.h"
#include "panic.h"
#include "printf.h"

typedef struct stackframe {
    struct stackframe *rbp;
    uint64_t rip;
} stackframe_t;

void panic_backtrace(uint64_t maxFrames) {
    stackframe_t *frame;
    asm("mov %%rbp,%0" : "=q"(frame) ::); // load RBP into the stack frame ptr

    for (uint64_t i = 0; frame && i < maxFrames; ++i) {
        printf("  0x%016llx\n", frame->rip);
        frame = frame->rbp;
    }
}

bool already_panicked = false;

__attribute__((noreturn))
void panic(const char *reason, registers_t *r) {
    if (already_panicked) {
        printf("*** PANIC called after/within panic\n");
        goto end;
    }
    already_panicked = true;

    printf("\n*** PANIC: %s ***\n", reason);

    if (r == NULL) {
        printf("\nRegister information unavailable\n");
    } else {
        printf(
            "Error code: %#llx\n"
            "\nRegisters:\n"
            "  RIP: 0x%016llx   RFLAGS: 0x%016llx\n"
            "  RAX: 0x%016llx      RBX: 0x%016llx\n"
            "  RCX: 0x%016llx      RDX: 0x%016llx\n"
            "  RSP: 0x%016llx      RBP: 0x%016llx\n"
            "  RSI: 0x%016llx      RDI: 0x%016llx\n"
            "   R8: 0x%016llx       R9: 0x%016llx\n"
            "  R10: 0x%016llx      R11: 0x%016llx\n"
            "  R12: 0x%016llx      R13: 0x%016llx\n"
            "  R14: 0x%016llx      R15: 0x%016llx\n"
            "   CS: 0x%016llx       SS: 0x%016llx\n",
            r->errorCode,
            r->rip, r->rflags,
            r->rax, r->rbx,
            r->rcx, r->rdx,
            r->rsp, r->rbp,
            r->rsi, r->rdi,
            r->r8, r->r9,
            r->r10, r->r11,
            r->r12, r->r13,
            r->r14, r->r15,
            r->cs, r->ss
        );
    }

    printf("\nBacktrace:\n");
    panic_backtrace(16);

end:
    printf("\n*** Halting now, good night.\n");
    for (;;) asm("hlt");
}

// implement SSP using panic
#define STACK_CHK_GUARD 0x595e9fbd94fda766

uint64_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void) {
    panic("stack smashing detected", NULL);
}

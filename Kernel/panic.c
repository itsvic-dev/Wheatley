#include "stdint.h"
#include "panic.h"
#include "printf.h"

#define READ_REGISTER(reg) asm volatile("mov %%" #reg ", %0" : "=q"(reg))

__attribute__((noreturn))
void panic(const char *reason) {
    printf("\n*** PANIC: %s ***\n", reason);
    printf("\n*** Halting now, good night.\n");
    for (;;) asm("hlt");
}

__attribute__((noreturn, no_caller_saved_registers))
void panic_interrupt(const char *reason, interrupt_frame_t *int_frame) {
    printf("\n*** PANIC from interrupt: %s ***\n", reason);

    if (int_frame != NULL) {
        printf(
            "\nRegisters:\n"
            "  RIP: 0x%016llx "
            "  RSP: 0x%016llx\n"
            "   CS: 0x%016llx\n",
            int_frame->ip,
            int_frame->sp,
            int_frame->cs
        );
    }
    
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

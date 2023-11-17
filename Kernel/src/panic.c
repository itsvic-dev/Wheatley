#include "panic.h"
#include "fw/madt.h"
#include "libk.h"
#include "printf.h"
#include "stdint.h"
#include "sys/apic.h"
#include "sys/cpuid.h"
#include "sys/spinlock.h"

typedef struct stackframe {
  struct stackframe *rbp;
  uint64_t rip;
} stackframe_t;

bootproto_symbol_t *find_symbol(uint64_t rip) {
  bootproto_symbol_t *bestFittingSymbol = 0;

  for (int i = 0; i < g_handoff->symbols_length; i++) {
    bootproto_symbol_t *symbol = &g_handoff->symbols[i];
    if (symbol->addr <= rip &&
        (bestFittingSymbol == 0 || symbol->addr >= bestFittingSymbol->addr)) {
      bestFittingSymbol = symbol;
    }
  }

  return bestFittingSymbol;
}

void panic_backtrace(uint64_t maxFrames) {
  stackframe_t *frame;
  asm("mov %0, rbp" : "=q"(frame)::); // load RBP into the stack frame ptr

  for (uint64_t i = 0; frame && i < maxFrames; ++i) {
    if (frame->rbp == 0 || frame->rip == 0)
      return;
    bootproto_symbol_t *symbol = find_symbol(frame->rip);
    if (symbol != 0)
      printf("  0x%016llx (%s+%#x)\n", frame->rip, symbol->name,
             frame->rip - symbol->addr);
    else
      printf("  0x%016llx (\?\?\?\?)\n", frame->rip);
    frame = frame->rbp;
  }
}

bool is_halting = false;
static spinlock_t lock = SPINLOCK_INIT;

static inline __attribute__((noreturn)) void halt() {
  for (;;) {
    asm("cli; hlt");
  }
}

__attribute__((noreturn)) void panic(const char *reason, registers_t *r) {
  asm("cli"); // we don't want any external interrupts to bother us during a
              // panic
  spinlock_wait_and_acquire(&lock);
  uint8_t apicID = getApicID();

  if (is_halting) {
    printf("*** PANIC called on core %d after/within panic\n", apicID);
    goto end;
  }
  is_halting = true;

  printf("\n*** PANIC on core %d: %s ***\n", apicID, reason);

  if (r == NULL) {
    printf("\nRegister information unavailable\n");
  } else {
    printf("Error code: %#llx\n"
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
           r->errorCode, r->rip, r->rflags, r->rax, r->rbx, r->rcx, r->rdx,
           r->rsp, r->rbp, r->rsi, r->rdi, r->r8, r->r9, r->r10, r->r11, r->r12,
           r->r13, r->r14, r->r15, r->cs, r->ss);
  }

  printf("\nBacktrace:\n");
  panic_backtrace(16);

  printf("\n*** Halting now, good night.\n");
  // tell all other CPUs that a panic has just happened
  apic_send_ipi(0, (0b100ul << 8) | (0b11ul << 18));
end:
  spinlock_release(&lock);
  halt();
}

void nmi_vector(registers_t *registers) {
  if (is_halting) {
    halt();
  } else {
    panic("Unexpected NMI", registers);
  }
}

// implement SSP using panic
#define STACK_CHK_GUARD 0x595e9fbd94fda766

uint64_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn)) void __stack_chk_fail(void) {
  panic("stack smashing detected", NULL);
}

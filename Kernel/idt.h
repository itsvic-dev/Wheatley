#ifndef __KERNEL_IDT_H
#define __KERNEL_IDT_H

#include "stdint.h"

typedef struct {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
   uint32_t offset_3;        // offset bits 32..63
   uint32_t zero;            // reserved
} __attribute__((__packed__)) idt_descriptor_t;

typedef enum {
    interrupt = 0b1110,
    trap = 0b1111,
} idt_gate_t;

typedef struct {
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
} interrupt_frame_t;

void setup_idt(void);

void idt_set_handler(uint8_t vector, idt_gate_t gate, void *isr);

#define INTERRUPT __attribute__((interrupt))

#endif // __KERNEL_IDT_H

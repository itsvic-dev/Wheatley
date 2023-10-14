#include "idt.h"
#include "printf.h"
#include "libk.h"
#include "assert.h"

idt_descriptor_t *idt_table;

uint16_t selector = 0b111000;

void setup_idt(void) {
    assert(sizeof(idt_descriptor_t) == 16);

    asm volatile("sidt idt_table");
    printf("idt: IDTR=0x%016llx\n", idt_table);

    uint64_t baseAddr = (uint64_t)idt_table >> 16;
    idt_table = (idt_descriptor_t *)baseAddr;
    printf("idt: idt_table=%#llx\n", baseAddr);

    printf("idt: clearing IDT table\n");
    memset(idt_table, 0, 256 * sizeof(idt_descriptor_t));
}

void idt_set_handler(uint8_t vector, idt_gate_t gate, void *isr) {
    idt_descriptor_t *desc = &idt_table[vector];
    uint64_t offset = (uint64_t)isr;
    desc->offset_1 = offset & 0xFFFF;
    desc->offset_2 = (offset >> 16) & 0xFFFF;
    desc->offset_3 = offset >> 32;
    desc->selector = selector;
    desc->ist = 0;
    
    uint8_t type_attributes = 0;
    // segment present flag
    type_attributes |= (1 << 7);
    // descriptor privilege level
    type_attributes |= (0b00 << 5);
    // type
    type_attributes |= gate & 0b1111;
    desc->type_attributes = type_attributes;
    printf("idt: handler for %#x @ %#llx registered\n", vector, offset);
}

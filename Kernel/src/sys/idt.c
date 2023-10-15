#include <sys/idt.h>
#include <assert.h>
#include <libk.h>
#include <printf.h>

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

void idt_set_handler(uint8_t vector, uint8_t ist, void *isr) {
    idt_descriptor_t *desc = &idt_table[vector];
    uint64_t offset = (uint64_t)isr;
    desc->offset_1 = offset & 0xFFFF;
    desc->offset_2 = (offset >> 16) & 0xFFFF;
    desc->offset_3 = offset >> 32;
    desc->selector = selector;
    desc->ist = ist;
    
    if (vector < 0x10) {
        // interrupts that kernel and usermode can handle
        if (((vector < 0xB) && (vector > 0x6)) || vector == 0x2)
            desc->type_attributes = 0x8F;
        else
            desc->type_attributes = 0xEF;
    } else {
        // interrupts only the kernel should handle
        desc->type_attributes = 0x8E;
    }
}

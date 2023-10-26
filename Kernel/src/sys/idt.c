#include <assert.h>
#include <libk.h>
#include <printf.h>
#include <sys/idt.h>

idt_descriptor_t idt_table[256];

idt_ptr_t idt_ptr = {0};

uint16_t selector = 0x8; // GDT kernel code

void idt_reload(void) {
  printf("idt: reloading\n");
  idt_ptr.limit = sizeof(idt_table) - 1;
  idt_ptr.addr = (uint64_t)&idt_table;
  asm("lidt idt_ptr");
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

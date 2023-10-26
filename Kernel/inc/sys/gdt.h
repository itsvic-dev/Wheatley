#ifndef __KERNEL_SYS_GDT_H
#define __KERNEL_SYS_GDT_H

#include <stdint.h>

// stolen from polaris to see if it restores my dwindling sanity
typedef struct {
  uint16_t limit;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_hi;

} __attribute__((__packed__)) gdt_entry_t;

typedef struct {
  uint16_t limit;
  uint64_t addr;
} __attribute__((__packed__)) gdt_ptr_t;

void gdt_write(void);
void gdt_reload(void);
extern gdt_ptr_t gdt_ptr;

#endif // __KERNEL_SYS_GDT_H

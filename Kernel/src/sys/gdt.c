#include <sys/gdt.h>
#include <printf.h>

// 6 entries:
//   - null
//   - kernel code
//   - kernel data
//   - user code
//   - user data
//   - tss
gdt_entry_t gdt_table[5];
gdt_ptr_t gdt_ptr = {0};

void gdt_write(void) {
    printf("gdt: writing gdt entries\n");
    
    // kernel code
    gdt_table[1].access = 0b10011010;
    gdt_table[1].granularity = 0b00100000;

    // kernel data
    gdt_table[2].access = 0b10010010;

    // user code
    gdt_table[3].access = 0b11111010;
    gdt_table[3].granularity = 0b00100000;

    // user data
    gdt_table[4].access = 0b11110010;

    gdt_ptr.limit = sizeof(gdt_table) - 1;
    gdt_ptr.addr = (uint64_t)&gdt_table;

    // TODO: install TSS
    gdt_reload();
}

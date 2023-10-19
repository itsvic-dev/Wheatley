#include <fw/apic.h>
#include <fw/acpi.h>
#include <panic.h>
#include <printf.h>
#include <assert.h>
#include <sys/io.h>

madt_t *madt;
uint32_t *lapic_addr = NULL;

void apic_init() {
    madt = (madt_t *)acpi_find_table("APIC", 0);
    if (!madt)
        panic("apic: no MADT found", 0);
    
    printf("apic: MADT @ %#llx, %#x bytes long\n", madt, madt->header.length);

    lapic_addr = (uint32_t *)(uint64_t)madt->lapic_address;

    // i love fucking with pointers
    madt_record_header_t *record = (madt_record_header_t *)((uint64_t)madt + 0x2C);
    while (record != NULL) {
        // this could've been a for loop
        switch(record->type) {
            case LAPIC_ADDRESS_OVERRIDE:
                lapic_addr = (uint32_t *)((madt_lapic_addr_override_t *)record)->address;
                printf("apic: LAPIC address overriden\n");
                break;
            default:
                printf("apic: unknown MADT record type %d\n", record->type);
                break;
        }

        record = (madt_record_header_t *)((uint64_t)record + record->length);
        if ((uint64_t)record >= (uint64_t)madt + madt->header.length) {
            record = NULL;
            break;
        }
    }

    printf("apic: LAPIC @ %#llx\n", lapic_addr);
    
    printf("apic: enabling LAPIC\n");
    // set SVI reg to 0xFF to enable the LAPIC
    // i *think* this will send spurious interrupts to 0xF0
    lapic_write(SPURIOUS_INTERRUPT_VECTOR_REG, 0xFF);

    printf("apic: disabling 8259 PIC\n");
    outb(0xA1, 0xFF);
    outb(0x21, 0xFF);
}

uint32_t lapic_read(size_t reg) {
    assert(lapic_addr != NULL);
    return lapic_addr[reg];
}
void lapic_write(size_t reg, uint32_t value) {
    assert(lapic_addr != NULL);
    lapic_addr[reg] = value;
}

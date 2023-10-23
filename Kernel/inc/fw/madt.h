#ifndef __KERNEL_FW_MADT_H
#define __KERNEL_FW_MADT_H

#include <acpispec/tables.h>

typedef struct {
    acpi_header_t header;

    uint32_t lapic_address;
    uint32_t flags;
} __attribute__((__packed__)) madt_t;

typedef enum : uint8_t {
    LAPIC,
    IOAPIC,
    IOAPIC_INTERRUPT_SOURCE_OVERRIDE,
    IOAPIC_NMI_SOURCE,
    LAPIC_NMI,
    LAPIC_ADDRESS_OVERRIDE,
    LOCALx2APIC = 9
} madt_record_type_t;

typedef struct {
    madt_record_type_t type;
    uint8_t length;
} __attribute__((__packed__)) madt_record_header_t;

typedef struct {
    madt_record_header_t header;
    uint8_t apicID;
    uint32_t flags;
} __attribute__((__packed__)) madt_lapic_t;

typedef struct {
    madt_record_header_t header;
    uint8_t ioapicID;
    uint8_t reserved;
    uint32_t address;
    uint32_t gsiBase;
} __attribute__((__packed__)) madt_ioapic_t;

typedef struct {
    madt_record_header_t header;
    uint8_t busSource;
    uint8_t irqSource;
    uint32_t gsi;
    uint16_t flags;
} __attribute__((__packed__)) madt_ioapic_iso_t;

typedef struct {
    madt_record_header_t header;
    uint8_t processor;
    uint16_t flags;
    uint8_t lint;
} __attribute__((__packed__)) madt_lapic_nmi_t;

typedef struct {
    madt_record_header_t header;
    uint16_t reserved;
    uint64_t address;
} __attribute__((__packed__)) madt_lapic_addr_override_t;

extern madt_t *madt;
extern uint32_t *lapic_addr;

extern madt_ioapic_iso_t *isos[256];
extern uint8_t isos_length;

extern madt_ioapic_t *ioapics[256];
extern uint8_t ioapics_length;

extern madt_lapic_nmi_t *nmis[256];
extern uint8_t nmis_length;

void madt_init();

#endif // __KERNEL_FW_MADT_H

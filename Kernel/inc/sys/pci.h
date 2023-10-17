#ifndef __KERNEL_SYS_PCI_H
#define __KERNEL_SYS_PCI_H

#include <stdint.h>
#include <acpispec/tables.h>

typedef struct {
    uint64_t baseAddr;
    uint16_t segment;
    uint8_t busStart;
    uint8_t busEnd;
    uint32_t reserved;
} __attribute__((__packed__)) mcfg_addr_t;

typedef struct {
    acpi_header_t header;
    uint8_t reserved[8];
    mcfg_addr_t addresses[];
} __attribute__((__packed__)) mcfg_t;

typedef struct {
    uint16_t seg;
    uint8_t bus;
    uint8_t slot;
    uint8_t fun;
} pci_device_path_t;

void pci_init(void);

void pci_outb(pci_device_path_t device, uint16_t offset, uint8_t val);
void pci_outw(pci_device_path_t device, uint16_t offset, uint16_t val);
void pci_outd(pci_device_path_t device, uint16_t offset, uint32_t val);

uint8_t pci_inb(pci_device_path_t device, uint16_t offset);
uint16_t pci_inw(pci_device_path_t device, uint16_t offset);
uint32_t pci_ind(pci_device_path_t device, uint16_t offset);

#endif // __KERNEL_SYS_PCI_H

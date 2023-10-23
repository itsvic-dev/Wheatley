#include <stdint.h>
#include <printf.h>
#include <panic.h>
#include <mm/mm.h>
#include <mm/internal.h>
#include <fw/acpi.h>
#include <sys/io.h>
#include <sys/pci.h>
#include <drivers/timer/hpet.h>

void *laihost_malloc(size_t size) {
    void *ret = kmalloc(size);
    return ret;
}
void *laihost_realloc(void *oldptr, size_t newsize, size_t oldsize) {
    void *ret = krealloc(oldptr, newsize);
    return ret;
}
void laihost_free(void *ptr, size_t size) {
    kfree(ptr);
}

void laihost_log(int level, const char *msg) {
    printf("lai (%d): %s\n", level, msg);
}

void *laihost_scan(char *sig, size_t index) {
    return acpi_find_table(sig, index);
}

__attribute__((noreturn)) void laihost_panic(const char *msg) {
    panic(msg, 0);
}

void laihost_outb(uint16_t port, uint8_t val) {
    outb(port, val);
}
void laihost_outw(uint16_t port, uint16_t val) {
    outw(port, val);
}
void laihost_outd(uint16_t port, uint32_t val) {
    outd(port, val);
}

uint8_t laihost_inb(uint16_t port) {
    return inb(port);
}
uint16_t laihost_inw(uint16_t port) {
    return inw(port);
}
uint32_t laihost_ind(uint16_t port) {
    return ind(port);
}

void laihost_pci_writeb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint8_t val) {
    pci_device_path_t device = {
        seg, bus, slot, fun
    };
    pci_outb(device, offset, val);
}
void laihost_pci_writew(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint16_t val) {
    pci_device_path_t device = {
        seg, bus, slot, fun
    };
    pci_outw(device, offset, val);
}
void laihost_pci_writed(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint32_t val) {
    pci_device_path_t device = {
        seg, bus, slot, fun
    };
    pci_outd(device, offset, val);
}

uint8_t laihost_pci_readb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
    pci_device_path_t device = {
        seg, bus, slot, fun
    };
    return pci_inb(device, offset);
}
uint16_t laihost_pci_readw(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
    pci_device_path_t device = {
        seg, bus, slot, fun
    };
    return pci_inw(device, offset);
}
uint32_t laihost_pci_readd(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
    pci_device_path_t device = {
        seg, bus, slot, fun
    };
    return pci_ind(device, offset);
}

void *laihost_map(size_t address, size_t count) {
    // its ok babe dont worry the physical address wont hurt you
    return (void *)address;
}

void laihost_unmap(void *ptr, size_t count) {
    // stub
    (void)ptr;
    (void)count;
}

// sleep for N ms
void laihost_sleep(uint64_t ms) {
    mdelay(ms);
}

uint64_t laihost_timer(void) {
    return hpet_time();
}

#include <stdint.h>
#include <printf.h>
#include <panic.h>
#include <mm/mm.h>
#include <mm/internal.h>
#include <fw/acpi.h>
#include <sys/io.h>

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

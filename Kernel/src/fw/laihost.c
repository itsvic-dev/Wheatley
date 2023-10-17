#include <stdint.h>
#include <printf.h>
#include <panic.h>
#include <mm/mm.h>
#include <mm/internal.h>
#include <fw/acpi.h>

void *laihost_malloc(size_t size) {
    return kmalloc(size);
}
void *laihost_realloc(void *oldptr, size_t newsize, size_t oldsize) {
    return krealloc(oldptr, newsize);
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

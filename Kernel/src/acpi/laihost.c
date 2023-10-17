#include <stdint.h>
#include <printf.h>
#include <mm/mm.h>

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

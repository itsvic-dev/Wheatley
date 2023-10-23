#include <sys/io.h>

void outb(uint16_t port, uint8_t val) {
    asm volatile(
        "out %1, %0"
        :
        : "a"(val), "Nd"(port)
        : "memory"
    );
}

void outw(uint16_t port, uint16_t val) {
    asm volatile(
        "out %1, %0"
        :
        : "a"(val), "Nd"(port)
        : "memory"
    );
}

void outd(uint16_t port, uint32_t val) {
    asm volatile(
        "out %1, %0"
        :
        : "a"(val), "Nd"(port)
        : "memory"
    );
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile(
        "in %0, %1"
        : "=a"(ret)
        : "Nd"(port)
        : "memory"
    );
    return ret;
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile(
        "in %0, %1"
        : "=a"(ret)
        : "Nd"(port)
        : "memory"
    );
    return ret;
}

uint32_t ind(uint16_t port) {
    uint16_t ret;
    asm volatile(
        "in %0, %1"
        : "=a"(ret)
        : "Nd"(port)
        : "memory"
    );
    return ret;
}


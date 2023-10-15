#include <drivers/tty/serialtty.h>
#include <drivers/tty/tty.h>
#include <sys/io.h>

#define COM1_PORT 0x3f8

int __serial_is_transmit_empty() {
   return inb(COM1_PORT + 5) & 0x20;
}

void serialtty_putchar(char a) {
   while (__serial_is_transmit_empty() == 0);
 
   outb(COM1_PORT, a);
}

tty_driver_t serialtty_driver;

void serialtty_module_init() {
    serialtty_driver.putchar = &serialtty_putchar;

    // init COM1
    outb(COM1_PORT + 1, 0x00); // Disable all interrupts
    outb(COM1_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(COM1_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1_PORT + 1, 0x00); //                  (hi byte)
    outb(COM1_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(COM1_PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(COM1_PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)
    
    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(COM1_PORT + 0) != 0xAE) {
        // don't continue initializing
        return;
    }
    
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(COM1_PORT + 4, 0x0F);
    
    tty_register_driver(&serialtty_driver);
}

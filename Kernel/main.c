#include "serial.h"

const char *hello = "hello, kernel world!";

void kernel_main(void) {
  if (!init_serial()) __asm__("int3");
  
  write_serial('h');
  write_serial('i');

  if (hello[0] == 0) {
    write_serial('y');
  } else {
    write_serial('n');
  }

  // write_serial(*hello);

  // const char *ptr = hello;
  // while (++ptr != 0) write_serial(*ptr);

  // write_serial('h');
  // write_serial('i');

  for (;;)
    __asm__("hlt");
}

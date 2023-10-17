#include <bootproto.h>
#include <printf.h>

#include <drivers/fb/fb.h>
#include <drivers/fb/efifb.h>
#include <drivers/tty/fbtty.h>
#include <drivers/tty/serialtty.h>

#include <mm/init.h>
#include <mm/mm.h>

#include <sys/cpuid.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <libk.h>
#include <panic.h>

#include <lai/core.h>
#include <lai/helpers/sci.h>

bootproto_handoff_t *g_handoff;

void kernel_main(bootproto_handoff_t *handoff) {
  g_handoff = handoff;

  // init early modules such as serialtty, efifb, fbtty
  serialtty_module_init();
  efifb_module_init();
  fbtty_module_init();

  gdt_write();
  isr_write();

  // init MM
  mm_init();

  cpuid_data_t cpuid_data = cpuid(0);
  get_cpuid_string(cpu_oem_id, &cpuid_data);
  printf("CPU manufacturer: %s\n", cpu_oem_id);

  char rsdpOemID[7];
  memcpy(rsdpOemID, g_handoff->rsdp.oemID, 6);
  rsdpOemID[6] = 0;
  printf("RSDP OEMID: %s (rev %d)\n", rsdpOemID, g_handoff->rsdp.revision);
  lai_set_acpi_revision(g_handoff->rsdp.revision);
  lai_create_namespace();
  lai_enable_acpi(1);
  
  panic("we're done for now", 0);
}

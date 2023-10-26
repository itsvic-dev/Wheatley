#include <bootproto.h>
#include <drivers/fb/efifb.h>
#include <drivers/fb/fb.h>
#include <drivers/tty/fbtty.h>
#include <drivers/tty/serialtty.h>
#include <fw/acpi.h>
#include <libk.h>
#include <mm/init.h>
#include <mm/mm.h>
#include <panic.h>
#include <printf.h>
#include <sched/sched.h>
#include <sys/apic.h>
#include <sys/cpuid.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/smp.h>

bootproto_handoff_t *g_handoff;

void kernel_main(bootproto_handoff_t *handoff) {
  g_handoff = handoff;

  // init early modules such as serialtty, efifb, fbtty
  serialtty_module_init();
  efifb_module_init();

  gdt_write();
  isr_write();

  // install panic's NMI vector
  isr_register_handler(2, nmi_vector);

  // init MM
  mm_init();

  // fbtty depends on MM
  fbtty_module_init();

  cpuid_data_t cpuid_data = cpuid(0);
  get_cpuid_string(cpu_oem_id, &cpuid_data);
  printf("CPU manufacturer: %s\n", cpu_oem_id);

  acpi_init();
  apic_init();
  sched_init();
  // smp init won't return as it enters deep main on all cores
  smp_init();
}

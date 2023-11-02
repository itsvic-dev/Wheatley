#include "mm/vmm.h"
#include <fw/acpi.h>
#include <panic.h>
#include <printf.h>
#include <sys/mmio.h>
#include <sys/pci.h>

mcfg_t *g_mcfg;

void pci_init(void) {
  mcfg_t *mcfg = (mcfg_t *)acpi_find_table("MCFG", 0);
  if (!mcfg || mcfg->header.length == 0)
    panic("pci: MCFG not supported", 0);

  g_mcfg = mcfg;
  printf("pci: MCFG @ %#llx\n", mcfg);
}

static void *pci_get_mcfg_addr(pci_device_path_t device, uint16_t offset) {
  // search through MCFG table
  // FIXME: this should be cached
  mcfg_addr_t *entry = NULL;
  size_t entries = (g_mcfg->header.length - sizeof(acpi_header_t) - 8) / 16;
  for (size_t i = 0; i < entries; i++) {
    mcfg_addr_t *e = &g_mcfg->addresses[i];
    if (e->segment == device.seg) {
      entry = e;
      break;
    }
  }
  // if we didnt find the segment, return null
  if (entry == NULL)
    return NULL;

  uint64_t addr = entry->baseAddr;
  addr += (device.bus - entry->busStart) << 20 | device.slot << 15 |
          device.fun << 12;
  addr += offset;
  vmm_map_page(vmm_get_kernel_pagemap(), addr, addr, 0b11);
  return (void *)addr;
}

void pci_outb(pci_device_path_t device, uint16_t offset, uint8_t val) {
  mmoutb(pci_get_mcfg_addr(device, offset), val);
}
void pci_outw(pci_device_path_t device, uint16_t offset, uint16_t val) {
  mmoutw(pci_get_mcfg_addr(device, offset), val);
}
void pci_outd(pci_device_path_t device, uint16_t offset, uint32_t val) {
  mmoutd(pci_get_mcfg_addr(device, offset), val);
}

uint8_t pci_inb(pci_device_path_t device, uint16_t offset) {
  return mminb(pci_get_mcfg_addr(device, offset));
}
uint16_t pci_inw(pci_device_path_t device, uint16_t offset) {
  return mminw(pci_get_mcfg_addr(device, offset));
}
uint32_t pci_ind(pci_device_path_t device, uint16_t offset) {
  return mmind(pci_get_mcfg_addr(device, offset));
}

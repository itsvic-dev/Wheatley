#include "assert.h"
#include "bmp3.h"
#include "bootproto.h"
#include "elf.h"
#include "elfloader.h"
#include "helpers.h"
#include "paging.h"
#include "printf.h"
#include <efi.h>
#include <protocol/efi-fp.h>
#include <protocol/efi-gop.h>
#include <protocol/efi-lip.h>
#include <protocol/efi-sfsp.h>
#include <stdint.h>

EFI_HANDLE Image;
EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES *BS;

bootproto_handoff_t *handoff = NULL;

typedef unsigned short wchar;

// printf needs this, but we dont plan to use floats in here so fuck this
void _fltused() {}

void _putchar(char c) {
  wchar wc[2];
  wc[0] = (wchar)c;
  wc[1] = 0;
  ST->ConOut->OutputString(ST->ConOut, wc);
}

EFI_STATUS GetImage(EFI_LOADED_IMAGE_PROTOCOL **image) {
  EFI_GUID guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  return BS->OpenProtocol(Image, &guid, (void **)image, Image, NULL,
                          EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

EFI_STATUS GetRootfs(EFI_HANDLE device,
                     EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **rootfs) {
  EFI_GUID guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

  return BS->OpenProtocol(device, &guid, (void **)rootfs, Image, NULL,
                          EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

int my_memcmp(const void *s1, const void *s2, int len) {
  unsigned char *p = (unsigned char *)s1;
  unsigned char *q = (unsigned char *)s2;
  int charCompareStatus = 0;
  // If both pointer pointing same memory block
  if (s1 == s2) {
    return charCompareStatus;
  }
  while (len > 0) {
    if (*p != *q) { // compare the mismatching character
      charCompareStatus = (*p > *q) ? 1 : -1;
      break;
    }
    len--;
    p++;
    q++;
  }
  return charCompareStatus;
}

bool guidMatches(EFI_GUID guid1, EFI_GUID guid2) {
  return my_memcmp(&guid1, &guid2, sizeof(EFI_GUID)) == 0;
}

void Panic(char *msg) {
  printf("panic: %s\r\n", msg);
  for (;;)
    asm("hlt");
}

uint64_t __kernelEntry;
void JumpToKernel(uint64_t stack);

bootproto_pmm_entry_type_t efi_type_to_bootproto_type[] = {
    reserved, reserved, reserved, free,     free,     used,     used,
    free,     reserved, reserved, reserved, reserved, reserved, reserved,
};

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st) {
  Image = image;
  ST = st;
  BS = ST->BootServices;

  ST->ConOut->ClearScreen(ST->ConOut);

  EFI_STATUS status;

  // get GOP
  EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
  status = BS->LocateProtocol(&gopGuid, NULL, (void **)&gop);
  STATUS_PANIC("failed to get GOP");

  bmp3_display(gop);

  // fuck write protection lololol
  __writecr0(__readcr0() & ~(1 << 16));

  uint64_t *curPage = (uint64_t *)__readcr3();

  EFI_LOADED_IMAGE_PROTOCOL *loadedImage;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *rootfs;
  EFI_FILE_PROTOCOL *rootdir;
  EFI_FILE_PROTOCOL *kernel;

  status = GetImage(&loadedImage);
  STATUS_PANIC("couldn't get LIP");
  status = GetRootfs(loadedImage->DeviceHandle, &rootfs);
  STATUS_PANIC("couldn't get SFSP");
  status = rootfs->OpenVolume(rootfs, &rootdir);
  STATUS_PANIC("couldn't get root FP");
  status =
      rootdir->Open(rootdir, &kernel, L"kernel.elf", EFI_FILE_MODE_READ, 0);
  STATUS_PANIC("couldn't get kernel.elf");
  printf("we have the kernel elf, lets try reading the elf header now\r\n");

  elf_header_t elfHeader;
  UINTN headerSize = sizeof(elf_header_t);
  status = kernel->Read(kernel, &headerSize, &elfHeader);
  STATUS_PANIC("failed to read ELF header from file");

  ASSERT(elfHeader.magic == ELF_MAGIC);

  printf("entry point: %#llx\r\n", elfHeader.entry);
  printf("program header offset: %#x\r\n", elfHeader.phdr_table);
  printf("phdr entry size: %#x\r\n", elfHeader.phdr_entry_size);
  printf("phdr entries n: %#x\r\n", elfHeader.phdr_entries_num);

  ASSERT(elfHeader.phdr_entry_size == sizeof(elf_phdr_t));

  // prepare handoff
  status = BS->AllocatePages(AllocateAnyPages, EfiRuntimeServicesData,
                             ((sizeof(bootproto_handoff_t)) / 4096) + 1,
                             (uint64_t *)&handoff);
  STATUS_PANIC("failed to allocate page for handoff");
  BS->SetMem(handoff, sizeof(bootproto_handoff_t), 0);

  // put handoff into the VMM sections
  handoff->vmm_entry[handoff->vmm_entries_length++] = (bootproto_vmm_entry_t){
      .physAddr = (uint64_t)handoff,
      .virtAddr = (uint64_t)handoff,
      .pages = ((sizeof(bootproto_handoff_t)) / 4096) + 1,
      .type = RW,
  };

  for (int i = 0; i < elfHeader.phdr_entries_num; i++) {
    status = kernel->SetPosition(kernel, elfHeader.phdr_table +
                                             (i * elfHeader.phdr_entry_size));
    STATUS_PANIC("failed to move file pos to phdr entry");

    elf_phdr_t phdr;
    UINTN size = elfHeader.phdr_entry_size;
    status = kernel->Read(kernel, &size, &phdr);
    STATUS_PANIC("failed to read phdr from file");

    if (phdr.type == elf_segt_null)
      continue;

    printf("----\r\n%s off %#llx vaddr %#llx align %d\r\n     filesz %#llx "
           "memsz %#llx flags %d\r\n",
           phdr.type == elf_segt_load ? "LOAD" : "????", phdr.offset,
           phdr.vaddr, phdr.alignment, phdr.filesz, phdr.memsz, phdr.flags);

    // this basic impl won't support any phdrs other than LOAD
    ASSERT(phdr.type == elf_segt_load);

    int pageCount = (phdr.memsz + phdr.alignment - 1) / phdr.alignment;

    // allocate physical page for phdr data
    void *physPage;
    status = BS->AllocatePages(AllocateAnyPages, EfiLoaderData, pageCount,
                               (uint64_t *)&physPage);
    STATUS_PANIC("failed to allocate memory");
    printf("allocated %d physical pages @ %#llx\r\n", pageCount, physPage);

    for (uint64_t z = 0; z < pageCount; z++) {
      uint64_t pageOffset = z * phdr.alignment;
      if (!Paging_MapPage(curPage, (uint64_t)physPage + pageOffset,
                          phdr.vaddr + pageOffset, 0b11)) {
        Panic("failed to map page");
      }
    }
    printf("mapped %d pages to virtual memory @ %#llx\r\n", pageCount,
           phdr.vaddr);

    handoff->vmm_entry[handoff->vmm_entries_length++] = (bootproto_vmm_entry_t){
        .physAddr = (uint64_t)physPage,
        .virtAddr = phdr.vaddr,
        .pages = pageCount,
        .type = RW,
    };

    BS->SetMem(physPage, phdr.memsz, 0);

    status = kernel->SetPosition(kernel, phdr.offset);
    STATUS_PANIC("failed to move file pos to phdr offset");

    size = phdr.filesz;
    status = kernel->Read(kernel, &size, physPage);
    STATUS_PANIC("failed to read segment data from file into memory");
  }

  // search through EFI config table to find RSDP
  xsdp_t *rsdp = NULL;
  EFI_GUID rdspGuid = EFI_ACPI_TABLE_GUID;
  for (int i = 0; i < ST->NumberOfTableEntries; i++) {
    EFI_CONFIGURATION_TABLE *ctEntry = &ST->ConfigurationTable[i];
    if (guidMatches(ctEntry->VendorGuid, rdspGuid)) {
      printf("found RSDP\r\n");
      rsdp = ctEntry->VendorTable;
    }
  }
  ASSERT(rsdp != NULL);

  // define stack
  void *stack = NULL;
  status =
      BS->AllocatePages(AllocateAnyPages, EfiLoaderData, 8, (uint64_t *)&stack);
  STATUS_PANIC("failed to allocate stack pages");

  for (int z = 0; z < 8; z++) {
    int pageOffset = z * 0x1000;
    if (!Paging_MapPage(curPage, (uint64_t)stack + pageOffset,
                        (uint64_t)stack + pageOffset, 0b11))
      Panic("failed to map the stack");
  }
  printf("new stack @ %#llx\r\n", stack);

  // put stack into VMM entries
  handoff->vmm_entry[handoff->vmm_entries_length++] = (bootproto_vmm_entry_t){
      .physAddr = (uint64_t)stack,
      .virtAddr = (uint64_t)stack,
      .pages = 8,
      .type = RW,
  };

  // read ELF symbol table and dump it into handoff
  elf_load_symbols(&elfHeader, kernel, handoff);

  // handoff->rsdp = *rsdp;
  BS->CopyMem(&handoff->rsdp, rsdp, rsdp->revision == 0 ? 20 : 36);

  // pass framebuffer info from GOP to handoff
  handoff->fb_buffer = (uint32_t *)gop->Mode->FrameBufferBase;
  handoff->fb_width = gop->Mode->Info->HorizontalResolution;
  handoff->fb_height = gop->Mode->Info->VerticalResolution;
  handoff->fb_pixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;
  printf("found GOP framebuffer %dx%d @ %#llx\r\n", handoff->fb_width,
         handoff->fb_height, handoff->fb_buffer);

  // get memory map and pass it off to handoff
  uint64_t MemoryMapSize = 0;
  EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
  uint64_t MapKey;
  uint64_t DescriptorSize;
  uint32_t DescriptorVersion;

  uint64_t totalMemSize = 0;

  BS->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize,
                   &DescriptorVersion);
  MemoryMapSize += DescriptorSize * 2;
  BS->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
  BS->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize,
                   &DescriptorVersion);

  for (int i = 0; i < (MemoryMapSize / DescriptorSize) / 2 + 1; i++) {
    EFI_MEMORY_DESCRIPTOR *memEntry =
        (EFI_MEMORY_DESCRIPTOR *)((uint64_t)MemoryMap + (i * DescriptorSize));

    handoff->pmm_entry[handoff->pmm_entries_length].type =
        efi_type_to_bootproto_type[memEntry->Type];
    handoff->pmm_entry[handoff->pmm_entries_length].start =
        memEntry->PhysicalStart;
    handoff->pmm_entry[handoff->pmm_entries_length++].pages =
        memEntry->NumberOfPages;

    totalMemSize += memEntry->NumberOfPages * 4096;
  }

  printf("total memory size: %d MB\r\n", totalMemSize / 1000 / 1000);

  printf("handoff ready @ %#llx\r\n", handoff);
  __kernelEntry = (uint64_t)elfHeader.entry;

  printf("exiting boot services and calling kernel. goodbye\r\n");
  BS->ExitBootServices(Image, MapKey);

  JumpToKernel((uint64_t)stack + (8 * 4096));

  return EFI_ERR;
}

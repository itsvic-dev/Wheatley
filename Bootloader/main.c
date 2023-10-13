#include <efi.h>
#include <protocol/efi-fp.h>
#include <protocol/efi-lip.h>
#include <protocol/efi-sfsp.h>
#include "elf.h"
#include "helpers.h"
#include "printf.h"
#include "paging.h"

EFI_HANDLE Image;
EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES *BS;

typedef unsigned short wchar;

// printf needs this, but we dont plan to use floats in here so fuck this
void _fltused() {}

void _putchar(char c) {
  wchar wc[2];
  wc[0] = (wchar) c;
  wc[1] = 0;
  ST->ConOut->OutputString(ST->ConOut, wc);
}

EFI_STATUS GetImage(
  EFI_LOADED_IMAGE_PROTOCOL **image
) {
  EFI_GUID guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  return BS->OpenProtocol(Image, &guid, (void **)image, Image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

EFI_STATUS GetRootfs(
  EFI_HANDLE device,
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **rootfs
) {
  EFI_GUID guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

  return BS->OpenProtocol(device, &guid, (void **)rootfs, Image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

void Panic(char *msg) {
  printf("panic: %s\r\n", msg);
  for (;;) __asm__("int3");
}

#define STATUS_PANIC(msg) if (status != EFI_SUCCESS) Panic(msg);
#define __STR(x) #x
#define STR(x) __STR(x)
#define ASSERT(cond) if (!(cond)) Panic("assert failed: " #cond " (on line " STR(__LINE__) ")")


typedef void (*kernel_entrypoint_t)(void);


EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st) {
  Image = image;
  ST = st;
  BS = ST->BootServices;

  ST->ConOut->ClearScreen(ST->ConOut);

  printf("hello, world!\r\n");
  EFI_STATUS status;

  printf("fucking write protection in the ass\r\n");
  __writecr0(__readcr0() & ~(1 << 16));

  uint64_t *curPage = (uint64_t *)__readcr3();
  printf("curPage: %#llx\r\n", curPage);

  EFI_LOADED_IMAGE_PROTOCOL *loadedImage;
  status = GetImage(&loadedImage);
  STATUS_PANIC("couldn't get LIP");

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *rootfs;
  status = GetRootfs(loadedImage->DeviceHandle, &rootfs);
  STATUS_PANIC("couldn't get SFSP");

  EFI_FILE_PROTOCOL *rootdir;
  status = rootfs->OpenVolume(rootfs, &rootdir);
  STATUS_PANIC("couldn't get root FP");

  EFI_FILE_PROTOCOL *kernel;
  status = rootdir->Open(rootdir, &kernel, L"kernel.elf", EFI_FILE_MODE_READ, 0);
  STATUS_PANIC("couldn't get kernel.elf");

  printf("we have the kernel elf, lets try reading the elf header now\r\n");

  elf_header_t elfHeader;
  UINTN headerSize = sizeof(elf_header_t);
  status = kernel->Read(kernel, &headerSize, &elfHeader);
  STATUS_PANIC("failed to read ELF header from file");

  uint8_t expectedMagic[4];
  expectedMagic[0] = 0x7f;
  expectedMagic[1] = 'E';
  expectedMagic[2] = 'L';
  expectedMagic[3] = 'F';

  if (!memcmp(elfHeader.magic, expectedMagic, 4)) {
    Panic("ELF magic does not match (or memcmp is faulty)");
  }

  printf("ELF magic matches!! :partying_face:\r\n");

  printf("entry point: %#llx\r\n", elfHeader.entry);
  printf("program header offset: %#x\r\n", elfHeader.phdr_table);
  printf("phdr entry size: %#x\r\n", elfHeader.phdr_entry_size);
  printf("phdr entries n: %#x\r\n", elfHeader.phdr_entries_num);

  ASSERT(elfHeader.phdr_entry_size == sizeof(elf_phdr_t));

  printf("lets read the phdr headers now\r\n");

  for (int i = 0; i < elfHeader.phdr_entries_num; i++) {
    status = kernel->SetPosition(kernel, elfHeader.phdr_table + (i * elfHeader.phdr_entry_size));
    STATUS_PANIC("failed to move file pos to phdr entry");

    elf_phdr_t phdr;
    UINTN size = elfHeader.phdr_entry_size;
    status = kernel->Read(kernel, &size, &phdr);
    STATUS_PANIC("failed to read phdr from file");

    if (phdr.type == elf_segt_null) continue;

    printf("----\r\n%s off %#llx vaddr %#llx align %d\r\n     filesz %#llx memsz %#llx flags %d\r\n",
      phdr.type == elf_segt_load ? "LOAD" : "????",
      phdr.offset,
      phdr.vaddr,
      phdr.alignment,
      phdr.filesz,
      phdr.memsz,
      phdr.flags);
    
    // this basic impl won't support any phdrs other than LOAD
    ASSERT(phdr.type == elf_segt_load);

    int pageCount = (phdr.memsz + phdr.alignment - 1) / phdr.alignment;

    // allocate physical page for phdr data
    void *physPage;
    status = BS->AllocatePages(AllocateAnyPages, EfiLoaderData, pageCount, (uint64_t *)&physPage);
    STATUS_PANIC("failed to allocate memory");
    printf("allocated %d physical pages @ %#llx\r\n", pageCount, physPage);

    for (uint64_t z = 0; z < pageCount; z++) {
      uint64_t pageOffset = z * phdr.alignment;
      if (!Paging_MapPage(curPage, (uint64_t)physPage + pageOffset, phdr.vaddr + pageOffset, 0b11)) {
        Panic("failed to map page");
      }
    }
    printf("mapped %d pages to virtual memory @ %#llx\r\n", pageCount, phdr.vaddr);
    
    // setmem can go to hell
    for (uint64_t i = 0; i < phdr.memsz; i++) {
      *(uint8_t *)(physPage + i) = 0;
    }
    
    status = kernel->SetPosition(kernel, phdr.offset);
    STATUS_PANIC("failed to move file pos to phdr offset");

    size = phdr.filesz;
    status = kernel->Read(kernel, &size, physPage);
    STATUS_PANIC("failed to read segment data from file into memory");

    printf("%#x\r\n", *(uint8_t *)phdr.vaddr);
  }

  kernel_entrypoint_t kernel_main = (kernel_entrypoint_t)elfHeader.entry;
  printf("let's jump to kernel\r\n");

  kernel_main();

  printf("if you see this we returned from kernel main, aka FAIL.\r\n");

  for (;;)
    ;

  return EFI_SUCCESS;
}

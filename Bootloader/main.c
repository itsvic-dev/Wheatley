#include <efi.h>
#include <protocol/efi-fp.h>
#include <protocol/efi-lip.h>
#include <protocol/efi-sfsp.h>
#include "elf.h"
#include "helpers.h"

#define NULL 0

EFI_HANDLE Image;
EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES *BS;

typedef unsigned short wchar;


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

void Panic(wchar *msg) {
  ST->ConOut->OutputString(ST->ConOut, L"panic: ");
  ST->ConOut->OutputString(ST->ConOut, msg);
  ST->ConOut->OutputString(ST->ConOut, L"\r\n");
  for (;;) __asm__("hlt");
}

#define PRINT(msg) ST->ConOut->OutputString(ST->ConOut, msg L"\r\n");
#define STATUS_PANIC(msg) if (status != EFI_SUCCESS) Panic(msg);

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st) {
  Image = image;
  ST = st;
  BS = ST->BootServices;

  PRINT(L"hello, world!");
  EFI_STATUS status;

  EFI_LOADED_IMAGE_PROTOCOL *loadedImage;
  status = GetImage(&loadedImage);
  STATUS_PANIC(L"couldn't get LIP");

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *rootfs;
  status = GetRootfs(loadedImage->DeviceHandle, &rootfs);
  STATUS_PANIC(L"couldn't get SFSP");

  EFI_FILE_PROTOCOL *rootdir;
  status = rootfs->OpenVolume(rootfs, &rootdir);
  STATUS_PANIC(L"couldn't get root FP");

  EFI_FILE_PROTOCOL *kernel;
  status = rootdir->Open(rootdir, &kernel, L"kernel.elf", EFI_FILE_MODE_READ, 0);
  STATUS_PANIC(L"couldn't get kernel.elf");

  PRINT(L"we have the kernel elf, lets try reading the elf header now");

  elf_header_t elfHeader;
  UINTN headerSize = sizeof(elf_header_t);
  status = kernel->Read(kernel, &headerSize, &elfHeader);
  STATUS_PANIC(L"failed to read ELF header from file");

  uint_8b expectedMagic[4];
  expectedMagic[0] = 0x7f;
  expectedMagic[1] = 'E';
  expectedMagic[2] = 'L';
  expectedMagic[3] = 'F';

  if (!memcmp(elfHeader.magic, expectedMagic, 4)) {
    Panic(L"ELF magic does not match (or memcmp is faulty)");
  }

  PRINT(L"ELF magic matches!! :partying_face:");

  for (;;)
    ;

  return EFI_SUCCESS;
}

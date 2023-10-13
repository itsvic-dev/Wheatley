#include <efi.h>

EFI_HANDLE image_handle;
EFI_SYSTEM_TABLE *system_table;

EFI_STATUS efi_main(EFI_HANDLE ih, EFI_SYSTEM_TABLE *st) {
  image_handle = ih;
  system_table = st;

  system_table->ConOut->OutputString(system_table->ConOut, L"hello, world!");

  return EFI_SUCCESS;
}

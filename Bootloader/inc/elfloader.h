#ifndef _ELFLOADER_H
#define _ELFLOADER_H

#include "bootproto.h"
#include "elf.h"
#include "protocol/efi-fp.h"

void elf_load_symbols(elf_header_t *header, EFI_FILE_PROTOCOL *file,
                      bootproto_handoff_t *handoff);

#endif // _ELFLOADER_H

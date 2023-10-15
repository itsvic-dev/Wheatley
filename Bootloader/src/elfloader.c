#include "elfloader.h"
#include "printf.h"
#include "assert.h"
#include <efi.h>

char *elf_shstrtab = NULL;
char *elf_strtab = NULL;
elf_symbol_t *elf_symbol_table = NULL;
int elf_symbols_count = 0;

extern EFI_BOOT_SERVICES *BS;

void elf_load_sections(elf_header_t *header, EFI_FILE_PROTOCOL *file) {
    ASSERT(header->shdr_entry_size == sizeof(elf_shdr_t));

    file->SetPosition(file, header->shdr_table);
    for (int i = 0; i < header->shdr_entries_num; i++) {
        UINTN size = header->shdr_entry_size;
        elf_shdr_t shdr;

        file->SetPosition(file, header->shdr_table + (size * i));
        file->Read(file, &size, &shdr);

        if (shdr.type == SHT_STRTAB && shdr.flags == 0 && elf_shstrtab == NULL) {
            printf("elf_load_sections: found shstrtab\r\n");
            BS->AllocatePool(EfiLoaderData, shdr.size, (void **)&elf_shstrtab);
            file->SetPosition(file, shdr.offset);
            size = shdr.size;
            file->Read(file, &size, elf_shstrtab);
        }
        else if (shdr.type == SHT_STRTAB && shdr.flags == 0 && elf_strtab == NULL) {
            printf("elf_load_sections: found strtab\r\n");
            BS->AllocatePool(EfiLoaderData, shdr.size, (void **)&elf_strtab);
            file->SetPosition(file, shdr.offset);
            size = shdr.size;
            file->Read(file, &size, elf_strtab);
        }

        if (shdr.type == SHT_SYMTAB) {
            printf("elf_load_sections: found symtab\r\n");
            BS->AllocatePool(EfiLoaderData, shdr.size, (void **)&elf_symbol_table);
            file->SetPosition(file, shdr.offset);
            size = shdr.size;
            file->Read(file, &size, elf_symbol_table);
            elf_symbols_count = shdr.size / sizeof(elf_symbol_t);
        }
    }
}

void elf_load_symbols(elf_header_t *header, EFI_FILE_PROTOCOL *file, bootproto_handoff_t *handoff) {
    if (!header->shdr_entries_num) {
        printf("elf_load_symbols: no section header\r\n");
    }
    elf_load_sections(header, file);

    ASSERT(elf_strtab != NULL);
    ASSERT(elf_symbol_table != NULL);

    // GLOBAL FUNC
    uint8_t expected_info = (1 << 4) | (2);

    handoff->symbols_length = 0;

    for (int i = 0; i < elf_symbols_count; i++) {
        elf_symbol_t *symbol = &elf_symbol_table[i];
        if (symbol->info != expected_info) continue;

        char *symname = elf_strtab + symbol->name;
        bootproto_symbol_t *bps = &handoff->symbols[handoff->symbols_length++];
        char *bpsName = bps->name;
        while ((*bpsName++ = *symname++) != 0);
        bps->addr = symbol->value;
    }

    printf("elf_load_symbols: loaded %d symbols into handoff\r\n", handoff->symbols_length);
}

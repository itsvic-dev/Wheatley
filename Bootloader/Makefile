CFLAGS := -target x86_64-pc-win32-coff -I$(PROJECT_ROOT)/ThirdParty/efi -fshort-wchar
LDFLAGS := -target x86_64-pc-win32-coff -Wl,-subsystem:efi_application -Wl,-entry:efi_main

PROJECT_NAME := Bootloader
TARGET_NAME := bootldr.efi
SRC := main.c helpers.c

# define PROJECT_NAME, SRC and TARGET_NAME before including cproj.mak!!!
include $(BS_ROOT)/cproj.mak

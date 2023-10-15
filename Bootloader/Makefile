CFLAGS := -target x86_64-pc-win32-coff -I$(PROJECT_ROOT)/ThirdParty/efi -fshort-wchar -fno-pie \
	-fno-stack-protector -fno-stack-check \

LDFLAGS := -target x86_64-pc-win32-coff -static -Wl,-subsystem:efi_application -Wl,-entry:efi_main

PROJECT_NAME := Bootloader
TARGET_NAME := bootldr.efi
SRC := main.c jump.asm helpers.c paging.c printf.c

# NASM needs to produce Win64 object files
override GLOBAL_ASFLAGS = -f win64

# define PROJECT_NAME, SRC and TARGET_NAME before including cproj.mak!!!
include $(BS_ROOT)/cproj.mak

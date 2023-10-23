CC := clang
AS := nasm

GLOBAL_CFLAGS := -g -Wall -Werror \
	-I$(PROJECT_ROOT)/Includes \
	-ffreestanding -mno-red-zone \
	-nostdlib -MD -masm=intel

GLOBAL_LDFLAGS := -fuse-ld=lld -nostdlib

GLOBAL_ASFLAGS := -g -f elf64

OUTPUT_DIRECTORY := $(PROJECT_ROOT)/build

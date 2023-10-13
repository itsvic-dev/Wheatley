# ComatOS makefile

PROJECT_ROOT := $(shell pwd)
BS_ROOT := $(PROJECT_ROOT)/BuildSystem

include $(BS_ROOT)/defines.mak
export

all: bootloader
.PHONY: all

bootloader:
	@echo -e " MAKE (Bootloader)"
	@$(MAKE) -C Bootloader build

clean:
	rm -rf build
.PHONY: clean


FAT32_IMG := $(OUTPUT_DIRECTORY)/ComatOS.img
fat32: all
	dd if=/dev/zero of=$(FAT32_IMG) bs=1M count=4
	mformat -i $(FAT32_IMG) ::
	mmd -i $(FAT32_IMG) ::/EFI
	mmd -i $(FAT32_IMG) ::/EFI/BOOT
	mcopy -i $(FAT32_IMG) $(OUTPUT_DIRECTORY)/Bootloader/bootldr.efi ::/EFI/BOOT/BOOTx64.efi
.PHONY: fat32

qemu: fat32
	qemu-system-x86_64 -enable-kvm -drive if=pflash,readonly=on,file=/usr/share/edk2-ovmf/OVMF_CODE.fd -hda $(FAT32_IMG) -serial stdio

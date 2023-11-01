#!/bin/bash
# Creates a FAT32 image with the ComatOS kernel and bootloader.

ROOT="$(realpath $(dirname $0)/..)"
BUILD="$ROOT/build"
IMG="$BUILD/ComatOS.img"

dd if=/dev/zero of="$IMG" bs=1M count=4 2>/dev/null
mformat -i "$IMG" ::
mcopy -i "$IMG" "$BUILD/Kernel/kernel.elf" ::/kernel.elf
mmd -i "$IMG" ::/EFI
mmd -i "$IMG" ::/EFI/BOOT
mcopy -i "$IMG" "$BUILD/Bootloader/bootldr.efi" ::/EFI/BOOT/BOOTx64.efi

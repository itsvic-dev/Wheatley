# ComatOS

[![Build](https://github.com/omametech/ComatOS/actions/workflows/build.yml/badge.svg)](https://github.com/omametech/ComatOS/actions/workflows/build.yml)

ComatOS is a small OS research project. It's not meant to be a serious alternative to Linux or Windows.

## Building

- Clone the repository. Make sure the submodules (in `ThirdParty`) were also cloned.
- Build it like any other CMake project.

## Running

To run in QEMU, use the `dev/qemu.sh` script. You can use the `.env` file to control the OVMF directory as well as extra QEMU arguments.

To run on real hardware (not recommended!), copy `build/Bootloader/bootldr.efi` to a FAT32 EFI partition, as well as `build/Kernel/kernel.elf` to the root of that partition.

If the kernel is not where it is expected (`PARTITION:/kernel.elf`), the bootloader will crash.

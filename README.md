# ComatOS

ComatOS is a small OS research project. It's not meant to be a serious alternative to Linux or Windows.

## Building

- Clone the repository. Make sure the submodules (in `ThirdParty`) were also cloned.
- Build it like any other CMake project.

## Running

To run in QEMU, you can use the helper target `qemu`. Simply `make qemu` and you're done!  
**Note:** this target expects OVMF to reside in `/usr/share/edk2-ovmf`. To change this, configure with `-DOVMF_PATH=/your/path/to/ovmf`.  
**Note:** to pass your own QEMU flags, configure with `"-DQEMU_ARGS=-your-args-here"`.

To run on real hardware (not recommended!), copy `build/Bootloader/bootldr.efi` to a FAT32 EFI partition, as well as `build/Kernel/kernel.elf` to the root of that partition.  
**Note:** If the kernel is not where it is expected (`PARTITION:/kernel.elf`), the bootloader will crash.

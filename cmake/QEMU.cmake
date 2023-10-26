add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/ComatOS.img
    COMMAND dd if=/dev/zero of=${CMAKE_BINARY_DIR}/ComatOS.img bs=1M count=4
    COMMAND mformat -i ${CMAKE_BINARY_DIR}/ComatOS.img ::
    COMMAND mcopy -i ${CMAKE_BINARY_DIR}/ComatOS.img ${CMAKE_BINARY_DIR}/Kernel/kernel.elf ::/kernel.elf
    COMMAND mmd -i ${CMAKE_BINARY_DIR}/ComatOS.img ::/EFI
    COMMAND mmd -i ${CMAKE_BINARY_DIR}/ComatOS.img ::/EFI/BOOT
    COMMAND mcopy -i ${CMAKE_BINARY_DIR}/ComatOS.img ${CMAKE_BINARY_DIR}/Bootloader/bootldr.efi ::/EFI/BOOT/BOOTx64.efi

    DEPENDS Kernel Bootloader
)

if(NOT OVMF_PATH)
    set(OVMF_PATH /usr/share/edk2-ovmf)
endif()
if(NOT QEMU_ARGS)
    set(QEMU_ARGS -m 128M -cpu host -enable-kvm -serial stdio -smp sockets=1,cores=4,threads=1)
endif()

add_custom_target(
    qemu
    COMMAND qemu-system-x86_64 -M q35 ${QEMU_ARGS} -drive if=pflash,readonly=on,file=${OVMF_PATH}/OVMF_CODE.fd -hda ${CMAKE_BINARY_DIR}/ComatOS.img
    DEPENDS ${CMAKE_BINARY_DIR}/ComatOS.img
)

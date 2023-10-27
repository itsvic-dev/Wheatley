project(ComatOSKernel)

add_executable(kernel.elf
    src/main.c src/deepMain.c src/libk.c src/panic.c src/printf.c
    
    src/fw/acpi.c src/fw/laihost.c src/fw/madt.c
    
    src/drivers/fb/fb.c src/drivers/fb/efifb.c
    src/drivers/timer/apic.c src/drivers/timer/hpet.c
    src/drivers/tty/tty.c src/drivers/tty/fbtty.c
    src/drivers/tty/serialtty.c

    src/mm/init.c src/mm/internal.c src/mm/mm.c src/mm/vmm.c

    src/sched/kernel.c src/sched/sched.c

    src/sys/apic.c src/sys/cpuid.c src/sys/gdt.asm src/sys/gdt.c
    src/sys/idt.c src/sys/io.c src/sys/isr.asm src/sys/isr.c
    src/sys/pci.c src/sys/pcrb.c src/sys/smp.asm src/sys/smp.c

    ${CMAKE_CURRENT_BINARY_DIR}/tamsyn.o
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tamsyn.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_OBJCOPY} -I binary -O elf64-x86-64 -B i386 Tamsyn8x16r.psf ${CMAKE_CURRENT_BINARY_DIR}/tamsyn.o
)

set_target_properties(kernel.elf PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)

target_compile_options(
    kernel.elf
    PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-fstack-protector -Wno-ignored-attributes>
)

target_link_options(
    kernel.elf
    PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld
)

target_include_directories(
    kernel.elf
    PRIVATE
    inc
)

target_link_libraries(kernel.elf lai)

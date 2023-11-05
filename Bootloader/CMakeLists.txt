set(CMAKE_C_COMPILER_TARGET "x86_64-pc-win32-coff")
set(CMAKE_EXECUTABLE_SUFFIX ".efi")
# NASM needs to produce Win64 object files
set(CMAKE_ASM_NASM_OBJECT_FORMAT "win64")
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

project(bootldr LANGUAGES C ASM_NASM)

add_executable(bootldr
    src/main.c src/bmp3.c src/elfloader.c src/jump.asm src/helpers.c src/paging.c
    src/printf.c
    "${CMAKE_CURRENT_BINARY_DIR}/logo.o"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/logo.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND objcopy -I binary -O pe-x86-64 -B i386 logo.bmp ${CMAKE_CURRENT_BINARY_DIR}/logo.o
)

target_compile_options(
    bootldr
    PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-fshort-wchar -fno-pie -fno-stack-protector -fno-stack-check>
)

target_link_options(
    bootldr
    PRIVATE
    -static -Wl,-subsystem:efi_application
    -Wl,-entry:efi_main
)

target_include_directories(
    bootldr
    PRIVATE
    inc
)

target_link_libraries(bootldr efiHeaders)

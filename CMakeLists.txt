cmake_minimum_required(VERSION 3.26)

# defaults
set(CMAKE_CROSSCOMPILING ON)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-Wall -Werror -ffreestanding -mno-red-zone -nostdlib -masm=intel")
set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64")
set(CMAKE_C_COMPILER_TARGET "x86_64-pc-none-elf")

project(ComatOS LANGUAGES C ASM_NASM)

add_link_options(-fuse-ld=lld -nostdlib)

include_directories(Includes)

add_subdirectory(ThirdParty)
add_subdirectory(Bootloader)
add_subdirectory(Kernel)

include(cmake/QEMU.cmake)

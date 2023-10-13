OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(kernel_main)

PHDRS
{
    text    PT_LOAD     FLAGS((1 << 0) | (1 << 2)); /* X-R */
    rodata  PT_LOAD     FLAGS((1 << 2));            /* --R */
    data    PT_LOAD     FLAGS((1 << 1) | (1 << 2)); /* -WR */
}

SECTIONS
{
    /* place us in the higher half */
    . = 0xffffffff80000000;

    .text : {
        *(.text .text.*)
    } :text

    . += CONSTANT(MAXPAGESIZE);

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    . += CONSTANT(MAXPAGESIZE);

    .data : {
        *(.data .data.*)
    } :data

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}

section .text

extern __kernelEntry
extern handoff

; called with MSx64 calling convention.
; kernel main is called with SysV calling convention.
global JumpToKernel
JumpToKernel:
    ; set the new stack
    mov rsp, rcx

    ; set rbp to null so backtraces will not include bootloader code
    xor rbp, rbp

    ; call main
    mov rdi, [handoff]
    call [__kernelEntry]

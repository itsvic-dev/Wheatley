extern ap_startup
extern bspDone
extern aprunning

global ap_trampoline
; the trampoline will be relocated to 0x8000
; sets up environment for calling a C function

%define CODE_SEG 0x08
%define DATA_SEG 0x10

[BITS 16]
ap_trampoline:
    cli
    cld
    ; load GDT stored at 0x9000
    lgdt [0x9000]

    ; some magic shit that deals with long mode i think
    mov ecx, 0xc0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr

    o32 jmp far [0x8020]

ALIGN 16
.farjmp:
    .offset dd 0x8040
    .segment dd 0x8

ALIGN 64
[BITS 64]
apLongMode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; get our LAPIC ID
    mov rax, 1
    cpuid
    shr rbx, 24
    mov rdi, rax
    ; set up 32k stack for each core
    shl rbx, 15
    mov rsp, 0x00EFFFFF
    sub rsp, rbx
    push rdi
.spinlock: ; wait for the BSP to finish
    mov rax, 0
    cmp [bspDone], rax
    jz .spinlock
    ; increment aprunning
    mov rax, [aprunning]
    inc rax
    mov [aprunning], rax
    ; jump to C code (shouldn't return)
    jmp ap_startup

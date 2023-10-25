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

    jmp 0x0000:0x8008

ALIGN 4
.flushCS:
    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov sp, 0x7000

    ; load empty IDT
    lidt [0x80c0]

    ; lets enter long mode!
    mov eax, 10100000b ; set the PAE and PGE bit
    mov cr4, eax

    mov edx, [0x9010] ; read CR3 value stored at 0x9010
    mov cr3, edx

    mov ecx, 0xC0000080
    rdmsr ; read the EFER MSR
    or eax, 0x00000100 ; set the LME bit
    wrmsr

    mov ebx, cr0
    or ebx, 0x80000001 ; enable paging and protection simultaneously
    mov cr0, ebx

    ; load fake GDT
    lgdt [0x8098]

    jmp CODE_SEG:0x8100 ; jump to apLongMode


ALIGN 64
GDT:
.null: dq 0
.code: dq 0x00209A0000000000; 64-bit code descriptor
.data: dq 0x0000920000000000; 64-bit data descriptor

ALIGN 4
.pointer:
    dw $ - GDT - 1
    dd 0x8080

ALIGN 64
IDT:
    .length dw 0
    .base dd 0

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

    mov rax, ap_startup
    jmp rax

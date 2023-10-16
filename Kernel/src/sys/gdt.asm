extern gdt_ptr

global gdt_reload
gdt_reload:
    lgdt [rel gdt_ptr]
    
    ; reload CS register
    push 0x8 ; code segment
    lea rax, [rel .reload_CS]
    push rax
    retfq ; far return to reload_CS
.reload_CS:
    mov eax, 0x10 ; data segment
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    ret

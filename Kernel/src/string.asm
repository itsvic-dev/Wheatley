; stolen from polaris which stole from limine

global memcpy
memcpy:
    mov rcx, rdx
    mov rax, rdi
    rep movsb
    ret

global memset
memset:
    push rdi
    mov rax, rsi
    mov rcx, rdx
    rep stosb
    pop rax
    ret

global memcmp
memcmp:
    mov rcx, rdx
    repe cmpsb
    je .equal

    mov al, byte [rdi-1]
    sub al, byte [rsi-1]
    movsx rax, al
    jmp .done

.equal:
    xor eax, eax

.done:
    ret

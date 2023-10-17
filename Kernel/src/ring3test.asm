global jump_to_ring3
jump_to_ring3:
    ;enable system call extensions that enables sysret and syscall
	mov rcx, 0xc0000082
	wrmsr
	mov rcx, 0xc0000080
	rdmsr
	or eax, 1
	wrmsr
	mov rcx, 0xc0000081
	rdmsr
	mov edx, 0x00180008
	wrmsr
 
	mov rcx, rdi ; to be loaded into RIP
	mov r11, 0x202 ; to be loaded into EFLAGS
	o64 sysret ; use "o64 sysret" if you assemble with NASM

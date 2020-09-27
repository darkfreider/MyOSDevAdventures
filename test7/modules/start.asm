section .bss
align 16

stack_bottom:
    resb (4 * 1024)
stack_top:

section .data
os_stack: dd 0

section .text

global _start
_start:

    mov dword [os_stack], esp
    mov esp, stack_top
 
    extern main
    call main

    mov esp, dword [os_stack]
    ret

    cli
.hang: 
    hlt
    jmp .hang



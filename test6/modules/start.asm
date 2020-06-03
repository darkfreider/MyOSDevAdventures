section .bss
align 16

stack_bottom:
    resb (4 * 1024)
stack_top:


section .text

global _start
_start:
    ;pushad
    ;push esp
    ;mov esp, stack_top

    extern main
    call main

    ;popad
    ;pop esp 
    ret

    cli
.hang: 
    hlt
    jmp .hang



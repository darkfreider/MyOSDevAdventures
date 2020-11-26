


; This is an entry point of the kernel
; After loading kernel in elf format bootloader jumps to _start lavel.
;
; We set up kernel stack and jump to kmain function.


section .bss
align 16

kernel_stack_bottom:
    resb (4 * 1024)
kernel_stack_top:

section .text

global _start
_start:

    mov esp, kernel_stack_top

    extern kmain
    push 0xaabbccdd
    call kmain

    cli
.hang:
    hlt
    jmp .hang

.end:

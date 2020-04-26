
org 0x7c00

BITS 16

first_instruction:
    jmp real_mode_start

align 4
pm_data: dd 0xdeadbeef
pm_msg:  db "Welcome to protected mode!", 0

align 8
gdt_start:
    gdt_null_seg_descr: 
        dq 0 
    
    gdt_code_seg_descr:
        .limit_0_15:     dw 0xffff
        .base_0_15:      dw 0
        .base_16_23:     db 0
        .seg_flags_low:  db 0b10011010 
        .seg_flags_high: db 0b11001111
        .base_24_31:     db 0 
    
    gdt_data_seg_descr:
        .limit_0_15:     dw 0xffff
        .base_0_15:      dw 0
        .base_16_23:     db 0
        .seg_flags_low:  db 0b10010010 
        .seg_flags_high: db 0b11001111
        .base_24_31:     db 0  
gdt_end:

gdt_descr:
gdt_descr_limit:     dw (gdt_end - gdt_start - 1) ;TODO(max): I need to check whether I need to do (-1) part, (I think I really need to do this, seen something about segmen limits in intel's manuals, BUT GDT is not a segmen (i think)
gdt_descr_base_addr: dd gdt_start


real_mode_start:
    cli
    
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    lgdt [gdt_descr]

    mov eax, cr0
    or eax, 0x01 
    mov cr0, eax

    jmp 0x08:protected_mode_start

BITS 32
protected_mode_start:
    mov ax, 0x10
    mov ds, ax 
    mov es, ax
    mov ss, ax
    
    xor ax, ax
    mov fs, ax
    mov gs, ax

    mov edi, 0xb8000
    mov esi, pm_msg
.print_str:
    lodsb
    or al, al
    jz .print_str_end
    mov byte [edi], al
    mov byte [edi + 1], 0b00000100
    add edi, 2
    jmp .print_str  
.print_str_end: 

    mov eax, [pm_data]    

.hang:
    jmp .hang

c_code:

;times 510 - ($-$$) db 0
;dw 0xaa55


; NOTE(max): compiling and execution
; nasm boot.asm -f bin -o boot.bin
; qemu-system-i386 -monitor stdio -drive format=raw,file=boot.bin

; Registers:
; eip
; eax, ebx, edx, ecx
; esi, edi, ebp, esp
; cs, ds, es, ss


org 0x7c00

BITS 16

first_instruction:
    jmp real_mode_start

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

%define PS2_DATA    0x60
%define PS2_STATUS  0x64
%define PS2_COMMAND 0x64

real_mode_start:
    cli

    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; NOTE(max): Disabling A20 line (no address wraparound)
.kbd_wait_0:
    in al, PS2_STATUS
    test al, 0x02
    jnz .kbd_wait_0

    mov al, 0xd1 ; NOTE(max): write to cntrlr output port
    out PS2_COMMAND, al

.kbd_wait_1:
    in al, PS2_STATUS
    test al, 0x02
    jnz .kbd_wait_1

    mov al, 0xdf
    out PS2_DATA, al

    lgdt [gdt_descr]

    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp 0x08:protected_mode_start

main_sp: dd 0
t0_sp: dd 0x7a00
t1_sp: dd 0x7800
t2_sp: dd 0x7600

BITS 32
protected_mode_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    xor ax, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x7c00
    mov ebp , esp

    mov eax, esp

    push t0_sp
    push t0_thread
    call init_stack_frame
    pop eax

    push t1_sp
    push t1_thread
    call init_stack_frame
    pop eax

    push t2_sp
    push t2_thread
    call init_stack_frame
    pop eax

    mov eax, main_sp
    mov ebx, t0_sp
    call switch
.hang:
    hlt
    jmp .hang

; ebp + 12 -> stack_addr
; ebp + 8 -> function ptr
; ebp + 4 -> ret addr
; ebp     -> old ebp
init_stack_frame:
    push ebp
    mov ebp, esp
    push eax
    push ebx

    mov eax, esp
    mov ebx, [ebp + 12]

    mov esp, [ebx]
    push dword [ebp + 8]
    push 0
    push 0
    push 0
    push 0
    push dword [ebx]
    push dword [ebx]
    push 0
    push 0
    pushfd
    mov [ebx], esp

    mov esp, eax

    pop ebx
    pop eax
    pop ebp
    ret

; ebp + 8 -> str ptr
; ebp + 4 -> ret addr
; ebp     -> old ebp
print_str:
    push ebp
    mov ebp, esp
    push eax
    push esi
    push edi

    mov esi, [ebp + 8]
    mov edi, 0xb8000

    xor eax, eax
.1:
    lodsb
    cmp al, 0
    je .done
    stosb
    mov al, 0x07
    stosb
    jmp .1

.done:
    pop edi
    pop esi
    pop eax
    pop ebp
    ret

; eax -> place to save old esp
; ebx -> switch to
switch:
    pushad
    pushfd

    mov [eax], esp
    mov esp, [ebx]

    popfd
    popad
    ret

t0_msg: db "Thread t0 is up!", 0
t0_thread:
    push t0_msg
    call print_str
    pop eax

    xor eax, eax
.1:
    cmp eax, 1000500000
    je .2
    inc eax
    jmp .1

.2:
    mov eax, t0_sp
    mov ebx, t1_sp
    call switch

    jmp t0_thread

t1_msg: db "Thread t1 is up!", 0
t1_thread:
    push t1_msg
    call print_str
    pop eax

    xor eax, eax
.1:
    cmp eax, 1000500000
    je .2
    inc eax
    jmp .1

.2:
    mov eax, t1_sp
    mov ebx, t2_sp
    call switch

    jmp t1_thread

t2_msg: db "Prepare for screen to be cleared!", 0
t2_thread:
    push t2_msg
    call print_str
    pop eax

    xor eax, eax
.1:
    cmp eax, 1000500000
    je .2
    inc eax
    jmp .1

.2:
    mov ax, (0x0700 | ' ')
    mov edi, 0xb8000
    mov ecx, 80 * 25
    rep stosw

    mov eax, t2_sp
    mov ebx, t0_sp
    call switch

    jmp t2_thread

times 510 - ($-$$) db 0
dw 0xaa55

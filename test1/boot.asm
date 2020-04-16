
%define CR 0x0d
%define NL 0x0a
%define BS 0x08


BITS 16
org 0x7c00


start:
    mov ax, 0x0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00 

.l1:
    mov si, prompt
    call print_string

    mov di, str_buffer
    call read_string
 
    mov si, str_buffer
    mov di, cmd_hello
    call str_cmp 
    jc .exec_cmd_hello
   
    mov si, str_buffer
    mov di, cmd_meaning_of_life
    call str_cmp
    jc .exec_cmd_meaning_of_life 
 
    jmp .l1

.exec_cmd_hello:
    mov si, msg_hello
    call print_string 
    jmp .l1

.exec_cmd_meaning_of_life:
    mov si, msg_meaning_of_life
    call print_string
    jmp .l1

.end:
    jmp .end

prompt db "@: ", 0
new_line db CR, NL, 0

cmd_hello db "hello", 0
msg_hello db "Welcome to os dude!", NL, CR, 0
cmd_meaning_of_life db "Meaning", 0
msg_meaning_of_life db "----{ 42 }----", NL, CR, 0 

%define STR_BUFFER_SIZE 64
str_buffer times STR_BUFFER_SIZE db 0


str_cmp:
    pusha
.l1:
    mov al, [si]
    cmp al, [di]
    jne .not_equal
    cmp al, 0
    je  .equal
 
    inc si
    inc di 
    jmp .l1 

.equal:
    stc
    jmp .end

.not_equal:
    clc
    jmp .end
 
.end:
    popa 
    ret

; @input:
;     di (): address of 64-byte buffer
read_string:
    pusha

    cld 
    xor cl, cl
.l1:
    mov ah, 0
    int 0x16

    cmp al, 0x08
    je .handle_backspace
    
    cmp al, CR
    je .done

    cmp cl, STR_BUFFER_SIZE - 1
    je .l1

    mov ah, 0x0e
    int 0x10

    stosb 
    inc cl
    jmp .l1
 
.handle_backspace:
    cmp cl, 0
    je .l1

    mov ah, 0x0e
    mov al, BS
    int 0x10
    mov al, ' '
    int 0x10
    mov al, BS
    int 0x10
    
    dec cl
    dec di 
    jmp .l1

.done:
    mov al, 0
    stosb

    mov ah, 0x0e
    mov al, CR
    int 0x10 
    mov al, NL
    int 0x10
     
.end:
    popa
    ret

; @input:
;     si (): address of src null-terminated string
print_string:
    pusha

    cld
.l1: 
    lodsb
    cmp al, 0
    je .end 
    mov ah, 0x0e
    int 0x10
    jmp .l1
.end:
    popa
    ret


times 510 - ($-$$) db 0
dw 0xaa55

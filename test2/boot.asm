
%define CR 0x0d
%define NL 0x0a
%define BS 0x08


BITS 16
org 0x7c00

%define STACK_SIZE 512

start:
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00


    mov word [tasks_array + (1 * 2)], 0x7c00 - (1 * STACK_SIZE)
    mov bx, [tasks_array + (1 * 2)]
    mov word [bx - 2], task_1_proc
    sub word [tasks_array + (1 * 2)], (2 + 16 + 2)

    mov word [tasks_array + (2 * 2)], 0x7c00 - (2 * STACK_SIZE)
    mov bx, [tasks_array + (2 * 2)]
    mov word [bx - 2], task_2_proc
    sub word [tasks_array + (2 * 2)], (2 + 16 + 2)

.l1:
    mov si, task_0_msg 
    call print_string

    mov ax, 0x8600
    mov cx, 0x0f
    mov dx, 0x4240
    int 0x15
   
    push 0
    push 1 
    call task_mn_switch
    add sp, 4

    mov si, task_0_msg_2
    call print_string

    mov ax, 0x8600
    mov cx, 0x0f
    mov dx, 0x4240
    int 0x15

    push 0
    push 1
    call task_mn_switch
    add sp, 4

    jmp .l1    
     

.end:
    jmp .end

task_1_proc:
.l1:
    mov si, task_1_msg
    call print_string
    
    mov ax, 0x8600
    mov cx, 0x0f
    mov dx, 0x4240
    mov dx, 2
    int 0x15

    push 1
    push 2
    call task_mn_switch
    add sp, 4
    

    jmp .l1 
    ret

task_2_proc:
.l1:
    mov si, task_2_msg
    call print_string
    
    mov ax, 0x8600
    mov cx, 0x0f
    mov dx, 0x4240
    mov dx, 2
    int 0x15

    push 2
    push 0
    call task_mn_switch
    add sp, 4
    

    jmp .l1 
    ret


tasks_array: times 5 dw 0

; stack:
;     from        (sp + 4)
;     to          (sp + 2)
;     ret_address (sp)
task_mn_switch:
    pushf
    pusha

    mov si, sp
    mov bx, [si + (16 + 2) + 4]
    shl bx, 1
    mov word [bx + tasks_array], sp
    mov bx, word [si + (16 + 2) + 2]
    shl bx, 1
    mov sp, word [bx + tasks_array]

    popa
    popf
    ret 



task_0_msg db "0:1", 0x0a, 0x0d, 0
task_0_msg_2 db "0:2", 0x0a, 0x0d, 0
task_1_msg db "1:1", 0x0a, 0x0d, 0
task_2_msg db "2:1", 0x0a, 0x0d, 0



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

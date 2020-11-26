
section .bss

section .text

; void thread_init_stack_frame(Thread_proc_t *func_ptr, uint32_t **stack_addr);
; TODO(max): I belive it can be done in C.
; -----------------------
; ebp + 12 -> stack_addr
; ebp + 8 -> function ptr
; ebp + 4 -> ret addr
; ebp     -> old ebp
; -----------------------
global thread_init_stack_frame
thread_init_stack_frame:
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

;void thread_switch(uint32_t **old_sp, uint32_t **new_sp);
; -----------------------
; esp + 8 + (9 * 4) -> new_sp
; esp + 4 + (9 * 4) -> old_sp
; esp + 0 + (9 * 4) -> ret addr
; -----------------------
global thread_switch
thread_switch:
switch:
    pushad
    pushfd

    mov eax, [esp + (9 + 1) * 4]
    mov ebx, [esp + (9 + 2) * 4]

    mov [eax], esp
    mov esp, [ebx]

    popfd
    popad
    ret

; void sheduler_start(uint32_t *sp);
; -----------------------
; ebp + 8 -> sp
; ebp + 4 -> ret addr
; ebp     -> old ebp
; -----------------------
global sheduler_start
sheduler_start:
    push ebp
    mov ebp, esp

    mov esp, [ebp + 8]
    popfd
    popad
    ret

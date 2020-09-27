


global syscall
; void syscall(uint32_t num, void *param)
; [ebp + 12] -- syscall parameter
; [ebp + 8]  -- syscall number 
; [ebp + 4]  -- ret addr
; [ebp + 0]  -- old ebp
syscall:
    push ebp
    mov ebp, esp
    push eax
    push ebx

    mov eax, dword [ebp + 8]  ; syscall num
    mov ebx, dword [ebp + 12] ; syscall param
    int 0x80

    pop ebx
    pop eax
    pop ebp
    ret


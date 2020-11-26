
org 0x7c00

BITS 16

first_instruction:
    jmp real_mode_start

    DEBUG_sectors_read dd 0

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
    cld

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


    push 0      ; offset
    push 4096   ; num_of_bytes
    push 0x1000 ; phys_addr
    call read_segment
    add esp, 12

%define ELF_MAGIC 0
%define ELF_ENTRY 24
%define ELF_PHOFF 28
%define ELF_PHNUM 44
%define SIZEOF_PROGHDR 32
%define LOG2_SIZEOF_PROGHDR 5
%define PHDR_PADDR 12
%define PHDR_FILESZ 16
%define PHDR_OFF 4
%define PHDR_MEMSZ 20

    ; ph, eax
    ; phe ebx
    ; elf ecx
    ; pa  edx
    mov dword [DEBUG_sectors_read], 0
    mov ecx, 0x1000

    mov eax, ecx
    add eax, dword [ecx + ELF_PHOFF]

    movzx ebx, word [ecx + ELF_PHNUM]
    shl ebx, LOG2_SIZEOF_PROGHDR
    add ebx, eax

.load_kernel_loop:
    cmp eax, ebx
    jnl .call_kernel_entry

    mov edx, dword [eax + PHDR_PADDR]

    push dword [eax + PHDR_OFF]
    push dword [eax + PHDR_FILESZ]
    push edx
    call read_segment
    add esp, 12

    ; use ecx as rep stos counter
    mov ecx, dword [eax + PHDR_MEMSZ]
    cmp ecx, dword [eax + PHDR_FILESZ]
    jng .l1

    mov edi, edx
    add edi, dword [eax + PHDR_FILESZ]
    sub ecx, dword [eax + PHDR_FILESZ]

    push eax
    xor eax, eax
    rep stosb
    pop eax

.l1:
    add eax, SIZEOF_PROGHDR
    jmp .load_kernel_loop

.call_kernel_entry:
    mov eax, dword [0x1000 + ELF_ENTRY]
    jmp eax

.hang:
    jmp .hang


%define LOG2_SECTOR_SIZE 9
%define SECTOR_SIZE (1 << LOG2_SECTOR_SIZE)

%define IDE_DATA          0x1f0
%define IDE_ERROR         0x1f1
%define IDE_SECTOR_COUNT  0x1f2
%define IDE_SECTOR_NUM    0x1f3
%define IDE_CYLINDER_LOW  0x1f4
%define IDE_CYLINDER_HIGH 0x1f5
%define IDE_SDH           0x1f6
%define IDE_STATUS        0x1f7
%define IDE_COMMAND       0x1f7
%define IDE_ALT_STATUS    0x3f6
%define IDE_DIGITAL_OUT   0x3f6
%define IDE_DRIVE_ADDR    0x3f7

%define IDE_STATUS_BUSY                (1 << 7)
%define IDE_STATUS_DRIVE_READY         (1 << 6)
%define IDE_STATUS_WRITE_FAULT         (1 << 5)
%define IDE_STATUS_DRIVE_SEEK_COMPLETE (1 << 4)
%define IDE_STATUS_DATA_REQUEST        (1 << 3)
%define IDE_STATUS_CORRECTABLE_DATA    (1 << 2)
%define IDE_STATUS_INDEX               (1 << 1)
%define IDE_STATUS_ERROR               (1 << 0)

; void read_sector(void *dest, uint sector_num);
; * Reads a single sector from the ide hard drive into
; * specified memory location (dest)
;
; Stack:
;     ebp + 12 -> sector_num
;     ebp + 8  -> dest
;     ebp + 4  -> return_addr
;     ebp      -> old_ebp
read_sector:
    push ebp
    mov ebp, esp

    push eax
    push edx
    push ecx
    push edi

    push eax
    mov eax, [ebp + 12]
    mov dword [DEBUG_sectors_read], eax
    ;inc dword [DEBUG_sectors_read]
    pop eax

    mov dx, IDE_STATUS
.wait_disk_0:
    in al, dx
    and al, 0xc0
    cmp al, 0x40
    jne .wait_disk_0

    mov dx, IDE_SECTOR_COUNT
    mov al, 1
    out dx, al

    mov dx, IDE_SECTOR_NUM
    mov eax, dword [ebp + 12]
    out dx, al

    mov dx, IDE_CYLINDER_LOW
    mov eax, dword [ebp + 12]
    shr eax, 8
    out dx, al

    mov dx, IDE_CYLINDER_HIGH
    mov eax, dword [ebp + 12]
    shr eax, 16
    out dx, al

    mov dx, IDE_SDH
    mov eax, dword [ebp + 12]
    shr eax, 24
    or al, 0xe0
    out dx, al

    mov dx, IDE_COMMAND
    mov al, 0x20
    out dx, al

    mov dx, IDE_STATUS
.wait_disk_1:
    in al, dx
    and al, 0xc0
    cmp al, 0x40
    jne .wait_disk_1

    ; TODO(max): maybe replace loop with a single instruction (rep stosw)
    mov dx, IDE_DATA
    mov cx, (SECTOR_SIZE / 2)
    mov edi, [ebp + 8]
.read_ide:
    in ax, dx
    stosw
    loop .read_ide

    pop edi
    pop ecx
    pop edx
    pop eax
    pop ebp
    ret


; void read_segment(uchar *phys_addr, uint num_of_bytes, uint offset)
;     * some documentation
; Stack:
;     ebp + 16 -> offset
;     ebp + 12 -> num_of_bytes
;     ebp + 8  -> phys_addr
;     ebp + 4  -> return_addr
;     ebp      -> old_ebp
;
read_segment:
    push ebp
    mov ebp, esp

    push eax
    push ebx
    push ecx
    push edx

    ; phys_addr     -> eax
    ; end_phys_addr -> ebx
    ; offset        -> ecx
    mov eax, [ebp + 8]

    ; IMPORTANT(max): assert(SECTOR_SIZE is a power of 2)
    ; x % y = x & (y - 1)
    ; Round down phys_addr to a sector boundary
    mov edx, [ebp + 16]
    and edx, (SECTOR_SIZE - 1)
    sub eax, edx

    ; translate offset in bytes to offset in sectors
    mov ecx, [ebp + 16]
    shr ecx, LOG2_SECTOR_SIZE
    ;inc ecx
    add ecx, 2

    mov ebx, [ebp + 8]
    add ebx, [ebp + 12]

.load_sectors:
    cmp eax, ebx
    jge .done

    push ecx
    push eax
    call read_sector
    add esp, 8

    add eax, SECTOR_SIZE
    inc ecx
    jmp .load_sectors

.done:
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret


times 506 - ($-$$) db 0
mata_block_start: dd 0
dw 0xaa55

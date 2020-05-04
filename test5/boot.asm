
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


real_mode_start:
    cli
    
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; TODO(max): activate A20 gate so that we will not be a memory
    ;            address wrapping

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

    mov esp, 0x07c00
    mov ebp , esp

    ; TODO(max): read kernel in elf format into memory

    ;push 1              ; sector_num
    ;push (0x1000) ; dest 
    ;call read_sector
    ;add esp, 8

    push 0      ; offset
    push 4096   ; num_of_bytes
    push 0x1000 ; phys_addr
    call read_segment
    add esp, 12

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
%define IDE_STATUS_ERROR               (1)

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

    mov dx, IDE_STATUS
.wait_disk_0: 
    in al, dx
    and al, 0xc0
    cmp al, IDE_STATUS_DRIVE_READY
    jne .wait_disk_0 

    mov dx, IDE_SECTOR_COUNT
    mov al, 1 

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
    cmp al, IDE_STATUS_DRIVE_READY
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
    inc ecx

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


times 510 - ($-$$) db 0
dw 0xaa55










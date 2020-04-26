
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

org 0x7c00

BITS 16
real_mode_start:
    
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00


    mov dx, IDE_SECTOR_COUNT
    mov al, 1 
    out dx, al

    mov dx, IDE_SECTOR_NUM
    mov al, 1
    out dx, al
    
    mov dx, IDE_CYLINDER_LOW
    mov al, 0
    out dx, al

    mov dx, IDE_CYLINDER_HIGH
    mov al, 0
    out dx, al

    mov dx, IDE_SDH
    mov al, (0xE0 | (0 << 4) | 0)
    out dx, al

    mov dx, IDE_COMMAND
    mov al, 0x20
    out dx, al

    mov dx, IDE_STATUS
.ide_wait:
    in al, dx
    test al, IDE_STATUS_BUSY
    jnz .ide_wait

    mov dx, IDE_DATA
    mov cx, 256
    mov di, (0x7c00 + 512) 
.read_ide:
    in ax, dx
    stosw 
    loop .read_ide


    jmp sector_1_start


times 510 - ($-$$) db 0
dw 0xaa55

;------------------------------------------------------ SECTOR 1

sector_1_start:
    mov si, sector_1_msg
    call print_string
 
    mov eax, 0xdeadbeef
.hang:
    jmp .hang

; @input:
;     si: addr of src null-terminated str
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

sector_1_msg: db "You've loaded sector 1 from IDE hard drive!", 0

times 1024 - ($-$$) db 0



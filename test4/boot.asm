
%define IDE_DATA          0x1f0
%define IDE_ERROR         0x1f1
%define IDE_SECTOR_COUNT  0x1f2
%define IDE_SECTOR_NUM    0x1f3
%define IDE_CYLINDER_LOW  0x1f4
%define IDE_CYLINDER_HIGH 0x1f5
%define IDE_SDH           0x1f6
%define IDE_STATUS        0x1f7
%define IDE_CMMAND        0x1f7
%define IDE_ALT_STATUS    0x3f6
%define IDE_DIGITAL_OUT   0x3f6
%define IDE_DRIVE_ADDR    0x3f7

org 0x7c00

BITS 16
real_mode_start:
    
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; TODO(max): read sector from disk and jump to sector_1_start

.hang:
    jmp .hang



times 510 - ($-$$) db 0
dw 0xaa55

;------------------------------------------------------ SECTOR 1

sector_1_start:
    mov ax, 0xbeef
.hang:
    jmp .hang

times 1024 - ($-$$) db 0



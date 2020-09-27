#!/bin/bash


# Build make_img programm that will concatinate bootloader (boot.bin) and
# my C kernel (kmain)
gcc -std=c99 -o make_img make_img.c

# assemble and compile kernel and bootloader
nasm boot.asm -f bin -o boot.bin

nasm -felf32 trap_asm.asm -o trap_asm.o
nasm -felf32 syscall.asm -o syscall.o
nasm -felf32 start.asm -o start.o
i686-elf-gcc -c kmain.c -o kmain.o -std=c99 -ffreestanding -O0 -Wall -Wextra

# link kernel
i686-elf-gcc -T linker.ld -o kmain -ffreestanding -O0 -nostdlib trap_asm.o start.o kmain.o -lgcc


# build all the helper programs that we'll need to create os
pushd ./modules/ >/dev/null
bash build_modules.sh
popd >/dev/null


# create disk image to run
./make_img


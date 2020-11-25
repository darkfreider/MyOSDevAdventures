#!/bin/bash


# Build make_img programm that will concatinate bootloader (boot.bin) and
# my C kernel (kmain)
gcc -std=c99 -o make_img make_img.c

# assemble and compile kernel and bootloader
nasm boot.asm -f bin -o boot.bin

nasm -felf32 start.asm -o start.o
nasm -felf32 thread.asm -o thread.o
i686-elf-gcc -g -c kmain.c -o kmain.o -std=c99 -ffreestanding -O0 -Wall -Wextra

# link kernel
i686-elf-gcc -T linker.ld -o kmain -ffreestanding -O0 -nostdlib start.o thread.o kmain.o -lgcc


# create disk image to run
./make_img

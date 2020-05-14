#!/bin/bash

# build all the helper programs that we'll need to create os
sh ./build_tools.sh

# assemble and compile kernel and bootloader
nasm boot.asm -f bin -o boot.bin

nasm -felf32 start.asm -o start.o
i686-elf-gcc -c kmain.c -o kmain.o -std=c99 -ffreestanding -O0 -Wall -Wextra

# link kernel
i686-elf-gcc -T linker.ld -o kmain -ffreestanding -O0 -nostdlib start.o kmain.o -lgcc

# create disk image to run
./make_img


#!/bin/bash

# build all the helper programs that we'll need to create os
sh ./build_tools.sh

# assemble and compile kernel and bootloader
nasm boot.asm -f bin -o boot.bin
i686-elf-gcc -c kmain.c -o kmain.o -std=c99 -ffreestanding -O0 -Wall -Wextra

# link kernel
i686-elf-gcc -T linker.ld -o kmain -ffreestanding -O0 -nostdlib kmain.o -lgcc

# create disk image to run
./make_img

# qemu-system-i386 -monitor stdio -drive format=raw,file=disk_image.bin


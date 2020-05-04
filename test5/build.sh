#!/bin/bash

# build all the helper programs that we'll need to create os
sh ./build_tools.sh

# assemble and compile kernel and bootloader
nasm boot.asm -f bin -o boot.bin
i686-elf-gcc kmain.c -o kmain -std=c99 -ffreestanding -nostdlib -O0 -Wall -Wextra

# create disk image to run
./make_img

qemu-system-i386 -monitor stdio -drive format=raw,file=disk_image.bin

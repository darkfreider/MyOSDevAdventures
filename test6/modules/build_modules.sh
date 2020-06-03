#!/bin/bash

nasm -felf32 start.asm -o start.o

i686-elf-gcc -c test.c -o test.o -std=c99 -ffreestanding -O0 -Wall -Wextra

# link 
i686-elf-gcc -T linker.ld -o test -ffreestanding -O0 -nostdlib start.o test.o -lgcc

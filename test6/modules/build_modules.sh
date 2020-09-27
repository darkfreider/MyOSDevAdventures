#!/bin/bash

nasm -felf32 start.asm -o start.o

i686-elf-gcc -c test.c -o test.o -std=c99 -ffreestanding -O0 -Wall -Wextra
i686-elf-gcc -c rule22.c -o rule22.o -std=c99 -ffreestanding -O0 -Wall -Wextra
i686-elf-gcc -c tx.c -o tx.o -std=c99 -ffreestanding -O0 -Wall -Wextra

# link
i686-elf-gcc -T linker.ld -o test -ffreestanding -O0 -nostdlib start.o test.o -lgcc
i686-elf-gcc -T linker.ld -o rule22 -ffreestanding -O0 -nostdlib start.o rule22.o -lgcc
i686-elf-gcc -T linker.ld -o tx -ffreestanding -O0 -nostdlib start.o tx.o -lgcc














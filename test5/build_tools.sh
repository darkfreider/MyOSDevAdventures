#!/bin/bash

# meta_elf is uset to generate elf specific %defines for boot.asm
gcc -std=c99 -o meta_elf meta_elf.c

# Build make_img programm that will concatinate bootloader (boot.bin) and
# my C kernel (kmain)
gcc -std=c99 -o make_img make_img.c

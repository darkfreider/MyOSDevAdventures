#!/bin/bash

# Build make_img programm that will concatinate bootloader (boot.bin) and
# my C kernel (kmain)
gcc -std=c99 -o make_img make_img.c

#!/bin/bash


qemu-system-i386 -monitor stdio -boot c -drive format=raw,file=disk_image.img 

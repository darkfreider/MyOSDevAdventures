#!/bin/bash

# -monitor stdio
qemu-system-i386 -boot c -drive format=raw,file=disk_image.img 

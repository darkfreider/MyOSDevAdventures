#!/bin/bash


qemu-system-i386 -monitor stdio -drive format=raw,file=disk_image.bin

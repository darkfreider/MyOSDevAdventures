# Asm thread switching
Here you can see how to implement thread(stack) switching in a boot loader.

---
## How to run:
nasm boot.asm -f bin -o boot.bin
qemu-system-i386 -monitor stdio -drive format=raw,file=boot.bin

nasm boot.asm -f bin -o boot.bin
qemu-system-i386 -monitor stdio -drive format=raw,file=boot.bin

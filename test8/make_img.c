



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#define ALIGN_DOWN(n, a) ((n) & ~((a) - 1))
#define ALIGN_UP(n, a) ALIGN_DOWN((n) + (a) - 1, (a))

#define SECTOR_SIZE 512

#define array_count(a) (sizeof(a) / sizeof((a)[0]))

off_t fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

int main(void)
{
    FILE *boot_bin   = fopen("boot.bin", "rb");
    FILE *kmain      = fopen("kmain", "rb");
    FILE *disk_image = fopen("disk_image.img", "wb");

    static char bootloader[SECTOR_SIZE];
    fread(bootloader, SECTOR_SIZE, 1, boot_bin);


    off_t elf_kernel_alloc_size = ALIGN_UP(fsize("kmain"), SECTOR_SIZE);
    char *elf_kernel = (char *)malloc(elf_kernel_alloc_size);
    fread(elf_kernel, fsize("kmain"), 1, kmain);

    fwrite(bootloader, SECTOR_SIZE, 1, disk_image);
    fwrite(bootloader, SECTOR_SIZE, 1, disk_image);
    fwrite(elf_kernel, elf_kernel_alloc_size, 1, disk_image);

    fclose(disk_image);
    fclose(kmain);
    fclose(boot_bin);

    return (0);
}

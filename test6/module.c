
#define SECTOR_SIZE 512

struct Module
{
    uint32_t index; // where a mudule is on a disk 
    uint8_t  name[12];
} __attribute__((packed));
typedef struct Module Module;

void
wait_disk(void)
{
    // Wait for disk ready.
    while((inb(0x1F7) & 0xC0) != 0x40)
        ;
}

// Read a single sector at offset into dst.
void
read_sector(void *dst, uint32_t offset)
{
  // Issue command.
  wait_disk();
  outb(0x1F2, 1);   // count = 1
  outb(0x1F3, offset);
  outb(0x1F4, offset >> 8);
  outb(0x1F5, offset >> 16);
  outb(0x1F6, (offset >> 24) | 0xE0);
  outb(0x1F7, 0x20);  // cmd 0x20 - read sectors

  // Read data.
  wait_disk();
  
  insl(0x1F0, dst, SECTOR_SIZE/4);
}

// Read 'count' bytes at 'offset' from kernel into physical address 'pa'.
// Might copy more than asked.
void
read_segment(uint8_t *pa, uint32_t count, uint32_t offset)
{
    uint8_t *epa;

    epa = pa + count;

    // Round down to sector boundary.
    pa -= offset % SECTOR_SIZE;

    // Translate from bytes to sectors; kernel starts at sector 1.
    offset = (offset / SECTOR_SIZE) + 1;

    for(; pa < epa; pa += SECTOR_SIZE, offset++)
        read_sector(pa, offset);
}

uint8_t scratch_space[4096];

int
load_and_execute(void)
{
    struct elfhdr *elf;
    struct proghdr *ph, *eph;
    void (*entry)(void);
    uint8_t *pa;

    elf = (struct elfhdr*)scratch_space;  // scratch space

    // Read 1st page off disk
    read_segment((uint8_t *)elf, 4096, 0);

    // Is this an ELF executable?
    if(elf->magic != ELF_MAGIC)
        return (0);

    // Load each program segment (ignores ph flags).
    ph = (struct proghdr*)((uint8_t *)elf + elf->phoff);
    eph = ph + elf->phnum;
    for(; ph < eph; ph++)
    {
        pa = (uint8_t *)ph->paddr;
        read_segment(pa, ph->filesz, ph->off);
        if(ph->memsz > ph->filesz)
            stosb(pa + ph->filesz, 0, ph->memsz - ph->filesz);
    }

    entry = (void(*)(void))(elf->entry);
    entry();

    return (1);
}
















// Routines to let C code use special x86 instructions.

uint8_t  
inb(uint16_t port)
{
  uint8_t data;

  __asm__ volatile("in %1,%0" : "=a" (data) : "d" (port));
  return data;
}

void
outb(uint16_t port, uint8_t data)
{
  __asm__ volatile("out %0,%1" : : "a" (data), "d" (port));
}

void
outw(uint16_t port, uint16_t data)
{
  __asm__ volatile("out %0,%1" : : "a" (data), "d" (port));
}

uint32_t 
reade_flags(void)
{
    uint32_t eflags;
    __asm__ volatile("pushfl; popl %0" : "=r" (eflags));
    return eflags;
}

void 
cli(void)
{
    __asm__ volatile("cli");
}

void
sti(void)
{
    __asm__ volatile("sti");
}









// Routines to let C code use special x86 instructions.

uint8_t  
inb(uint16_t port)
{
    uint8_t data;

    __asm__ volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

uint32_t
inl(uint16_t port)
{
    uint32_t data;

    __asm__ volatile("in %1, %0" : "=a" (data) : "d" (port));
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

void
outl(uint16_t port, uint32_t data)
{
    __asm__ volatile("out %0, %1" : : "a" (data), "d" (port));
}

void
insl(int port, void *addr, int cnt)
{
  __asm__ volatile("cld; rep insl" :
               "=D" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "memory", "cc");
}

void
stosb(void *addr, int data, int cnt)
{
  __asm__ volatile("cld; rep stosb" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

uint32_t 
reade_flags(void)
{
    uint32_t eflags;
    __asm__ volatile("pushfl; popl %0" : "=r" (eflags));
   
    return eflags;
}

void
load_idt(Interrupt_gate *p, uint16_t size)
{
    volatile uint16_t idtr[3];

    idtr[0] = size - 1;
    idtr[1] = (uint16_t)(p);
    idtr[2] = (uint32_t)(p) >> 16;

    __asm__ volatile("lidt (%0)" :: "r" (idtr));
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







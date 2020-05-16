


#ifndef __TRAP_H__
#define __TRAP_H__

#include <stdint.h>

#define PIC1_CMD 0x20
#define PIC1_DATA (PIC1_CMD + 1)
#define PIC2_CMD 0xa0
#define PIC2_DATA (PIC2_CMD + 1)

#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x1

#define PIC1_NEW_OFFSET 0x20
#define PIC2_NEW_OFFSET 0x28 

struct Trap_frame
{
    // NOTE(max): registers pushed by pusha
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t oesp; 
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t trap_num;

    // NOTE(max): hardware pushed registers    
    uint32_t err_num;
    uint32_t eip;
    uint16_t cs;
    uint16_t padding_0; 
    uint32_t eflags; 
};
typedef struct Trap_frame Trap_frame;

struct Interrupt_gate
{
    uint16_t offset_0_15;
    uint16_t selector;

    uint8_t zero;
    uint8_t type; 

    uint16_t offset_16_31;
} __attribute__((packed));
typedef struct Interrupt_gate Interrupt_gate;

extern Interrupt_gate idt[256];

#define IRQ_BASE 32
#define IRQ_0 IRQ_BASE + 0
#define IRQ_1 IRQ_BASE + 1

void pic_eoi(uint8_t);
void init_interrupt_gate_entry(Interrupt_gate *, uint32_t, uint16_t);
void trap_init(void);
void trap(Trap_frame *);



inline void trap_static_asserts(void)
{
    static_assert(sizeof(Interrupt_gate) == 8);
}

#endif












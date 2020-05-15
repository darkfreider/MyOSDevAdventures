

#include <stdint.h>

#ifndef __TRAP_H__
#define __TRAP_H__

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

inline void trap_static_asserts(void)
{
    static_assert(sizeof(Interrupt_gate) == 8);
}

#endif












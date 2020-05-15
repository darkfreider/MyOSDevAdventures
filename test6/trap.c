
#include "trap.h"

Interrupt_gate idt[256];
extern uint32_t vectors_array[]; 

void 
init_interrupt_gate_entry(Interrupt_gate *entry, uint32_t offset, uint16_t selector)
{
    entry->offset_0_15  = 0xffff & offset;
    entry->offset_16_31 = 0xffff & (offset >> 16);
    entry->selector     = selector;
    entry->zero         = 0;
    entry->type         = 0x8e;
}

void test_trap_handler(void)
{
    vga_clear_screen();
    vga_print_message("Hello from trap!", 0x07, 0, 0);    
}

void 
trap_init(void)
{
    for (int i = 0; i < 256; i++)
    {
        init_interrupt_gate_entry(&idt[i], vectors_array[i], 0x08);
    }

    init_interrupt_gate_entry(&idt[32], (uint32_t)test_trap_handler, 0x08);

    load_idt(idt, sizeof(idt));
}



void 
trap(Trap_frame *frame)
{
    vga_print_message("trap()", 0x07, 0, 24);
    if (frame->trap_num == 32)
    {
        test_trap_handler(); 
    }

    return;
}





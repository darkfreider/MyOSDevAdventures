
#include "trap.h"

void
pic_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(PIC2_CMD, PIC_EOI); 
    }
    outb(PIC1_CMD, PIC_EOI);
}

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


void 
trap_init(void)
{
    // NOTE(max): We initialize IDT
    for (int i = 0; i < 256; i++)
    {
        init_interrupt_gate_entry(&idt[i], vectors_array[i], 0x08);
    }
    load_idt(idt, sizeof(idt));
 
    // NOTE(max): ICW1
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
	
    // NOTE(max): ICW2 
    outb(PIC1_DATA, PIC1_NEW_OFFSET); 
    outb(PIC2_DATA, PIC2_NEW_OFFSET);
	
    // NOTE(max): ICW3
    outb(PIC1_DATA, 0x4);
    outb(PIC2_DATA, 0x2);
	
    // NOTE(max): ICW4
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // NOTE(max): mask ALL hw interrupts
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void 
test_trap_handler(void)
{ 
    vga_print_message("Hello from trap LOLOL!", 0x07, 0, 1);    
}

void 
trap(Trap_frame *frame)
{
    vga_print_message("trap()", 0x07, 0, 0);

    switch (frame->trap_num)
    {
        case 0x80:
	{
            test_trap_handler();	
	} break;

        case IRQ_1: // keyboard
        {
	    pic_eoi(1);

            uint8_t scan_code = inb(0x60);
	    char msg[2] = { scan_code, 0 };
	    vga_clear_screen();
            vga_print_message(msg, 0x07, 0, 0); 
	} break;	
    
        default:
	{
	    vga_clear_screen();
            vga_print_message("I can't handle this interrupt", 0x07, 0, 0);	
	} break;	
    } 
}





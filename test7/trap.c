
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
test_trap_handler(uint32_t param)
{
    printf("Syscall param = %x\n", param);	
}

void
syscall_meaning_of_life(void)
{
    printf("The meaning of life is 42\n");
}

void
syscall_print_str(char *str)
{
    put_str(str);
}

void 
trap(Trap_frame *frame)
{
    switch (frame->trap_num)
    {
        case 0x80:
	{
	    if (frame->eax == 42)
	        syscall_meaning_of_life();
	    else if (frame->eax == 7)
	        syscall_print_str((char *)frame->ebx);
	} break;

        case IRQ_1: // keyboard
        {
	    pic_eoi(1);
            kbd_handler();
	} break;	
    
        default:
	{
	    vga_clear_screen();
            put_str("I can't handle this interrupt\n");
	} break;	
    } 
}





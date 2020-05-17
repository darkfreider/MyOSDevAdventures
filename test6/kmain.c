
#if defined(__linux__)
#error "Your are not using cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This should be compiled with ix86 compiler"
#endif

#define static_assert(e) do { switch(0) { case 0: case (e):break; } } while(0)

#include <stdint.h>

#include "trap.h"
#include "vga.h"
#include "ps2.h"

void runtime_assert(int e, const char *msg)
{
    if (e == 0)
    {
        vga_clear_screen();
        put_str(msg); 
    }
}


#include "x86.c"
#include "vga.c"
#include "trap.c"
#include "ps2.c"

char g_attr = 0x27;

int arr[4] = {0xdeadbeef, 0xbeefdead, 0xdeaddead, 0xbeefbeef};

char *msg[] = { 
    "hello", 
    "world",
    "c kernel",
    "works!",
    "Holy shit! I've found this error!!!!"
};

int kmain(uint32_t magic)
{
    vga_set_text_color(BLACK, GREY);  
    vga_clear_screen();
    
    trap_init();
    ps2_controller_init();
    
    uint8_t new_mask = inb(PIC1_DATA) & ~(1 << 1);
    outb(PIC1_DATA, new_mask); 
    sti(); 
    
    __asm__ volatile("int $0x80");   
   
    {    
        
        put_str("Return from trap!\n");
    
        print_hex(inb(PIC1_DATA));
        put_char('\n'); 
	print_hex(inb(PIC2_DATA));
	put_char('\n');
    }

    for(;;);

    return (0);
}














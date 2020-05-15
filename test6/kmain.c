
#if defined(__linux__)
#error "Your are not using cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This should be compiled with ix86 compiler"
#endif

#include <stdint.h>

#define static_assert(e) do { switch(0) { case 0: case (e):break; } } while(0)

// TODO(max): create .h files for .c files
#include "trap.h"

#include "x86.c"
#include "vga.c"
#include "trap.c"


char g_attr = 0x27;

int arr[4] = {0xdeadbeef, 0xbeefdead, 0xdeaddead, 0xbeefbeef};

void print_hex(int h, char attr, int x, int y)
{
    char *hex_to_char = "0123456789abcdef";
    char out[] = "0x00000000 ";

    out[2] = hex_to_char[(h >> 28) & 0xf];
    out[3] = hex_to_char[(h >> 24) & 0xf];

    out[4] = hex_to_char[(h >> 20) & 0xf];
    out[5] = hex_to_char[(h >> 16) & 0xf];

    out[6] = hex_to_char[(h >> 12) & 0xf];
    out[7] = hex_to_char[(h >> 8) & 0xf];
    
    out[8] = hex_to_char[(h >> 4) & 0xf];
    out[9] = hex_to_char[(h >> 0) & 0xf];

    vga_print_message(out, attr, x, y);
}

char *msg[] = { 
    "hello", 
    "world",
    "c kernel",
    "works!",
    "Holy shit! I've found this error!!!!"
};

int kmain(uint32_t magic)
{
    trap_init();

    vga_clear_screen();
    vga_move_cursor(0, 0);

    for (int i = 0; i < 5; i++)
	    vga_print_message(msg[i], g_attr, 0, i);
 

    print_hex(magic, 0x07, 0, 20); 

    
    __asm__ volatile("int $32"); 

    vga_print_message("Return from trap!", 0x07, 0, 2);
    for (int i = 0; i < 4; i++)
	   print_hex(arr[i], 0x07, 0, i + 4); 
    
    for (;;);
  
    return (0);
}

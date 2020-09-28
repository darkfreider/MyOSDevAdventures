
#if defined(__linux__)
#error "Your are not using cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This should be compiled with ix86 compiler"
#endif

#define static_assert(e) do { switch(0) { case 0: case (e):break; } } while(0)

#include <stdint.h>
#include <stdarg.h>

#include "vga.h"

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


void 
kmain(uint32_t magic)
{
    vga_set_text_color(BLACK, GREY);  
    vga_clear_screen();
    
    printf("Hello from kernel!\n");

    for(;;);
}














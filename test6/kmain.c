
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
#include "kbd.h"

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
#include "kbd.c"

#define INPUT_BUFFER_SIZE 16 
char g_input_buf[INPUT_BUFFER_SIZE];
int  g_input_buf_next_write = 0;
int  g_input_buf_next_read  = 0;

int 
is_print(uint8_t c)
{
    return (c >= 0x20) && (c <= 0x7e);
}

int 
to_lower(uint8_t c)
{
    return ((c >= 'A') && (c <= 'Z')) ? (c + 32) : c;
}

uint8_t 
get_char(void)
{
    if (g_input_buf_next_write == g_input_buf_next_read)
    {
        g_input_buf_next_read = 0;
	g_input_buf_next_write = 0;

        for (;;)
	{
            KBD_msg msg = get_kbd_msg();
	    if (msg.msg == MSG_KEYDOWN)
	    {
	        if (is_print(msg.vk))
		{
		    if (g_input_buf_next_write < (INPUT_BUFFER_SIZE - 1))
		    {
	                char c = (msg.flags & 0x01) ? msg.vk : to_lower(msg.vk);
		        put_char(c);

			g_input_buf[g_input_buf_next_write++] = c;
	            }	
		}
	        else if ((msg.vk == '\b') && g_input_buf_next_write)
		{
	            put_char('\b');
	            put_char(' ');
	            put_char('\b');

		    g_input_buf_next_write--;
		}
	        else if (msg.vk == '\n')
		{
		    if (g_input_buf_next_write < INPUT_BUFFER_SIZE)
		    {
		        put_char('\n');
			g_input_buf[g_input_buf_next_write++] = '\n';
                        break; 
		    }
		}	
	    }
	}
    }

    return g_input_buf[g_input_buf_next_read++]; 
}

int 
get_line(char *str, int len)
{
    int c;
    int i = 0;
	
    while ((c = get_char()) != '\n' && i < (len - 1))
    {
        str[i++] = c;
    }
    if (c == '\n')
    {
        str[i++] = '\n';
    }
    str[i] = '\0';
	
    return (i);
}

void 
shell(void)
{
    char str[64];
    for (;;)
    {
        get_line(str, sizeof(str));
	put_str(str);
	//put_char('\n');
    }
}

int 
kmain(uint32_t magic)
{
    vga_set_text_color(BLACK, GREY);  
    vga_clear_screen();
    
    trap_init();
    ps2_controller_init();
    
    uint8_t new_mask = inb(PIC1_DATA) & ~(1 << 1);
    outb(PIC1_DATA, new_mask); 
    sti(); 

    shell();


    for(;;);

    return (0);
}














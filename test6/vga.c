
uint16_t *g_console_memory = (uint16_t *)0xb8000;
int g_text_attr = 0x07;
int g_cursor_x = 0;
int g_cursor_y = 0;


void 
vga_set_text_color(uint8_t fg, uint8_t bg)
{
    g_text_attr = (bg << 4) | (fg & 0x0F);
}

void 
vga_move_cursor(int x, int y)
{	
    g_cursor_x = x;
    g_cursor_y = y;
	
    uint16_t temp = y * SCREEN_WIDTH + x;
	
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

void
vga_clear_screen(void)
{	
    uint16_t blank = ' ' | (g_text_attr << 8);
	
    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
    {
        g_console_memory[i] = blank;
    }
	
    vga_move_cursor(0, 0);
}

void
vga_scroll_screen(void)
{
	
    if (g_cursor_y >= SCREEN_HEIGHT)
    {
        int i;
	uint16_t blank = ' ' | (g_text_attr << 8);
		
	uint16_t chars_to_copy = ((SCREEN_HEIGHT - 1) * SCREEN_WIDTH);
	uint16_t *dest = g_console_memory;
	uint16_t *src = g_console_memory + SCREEN_WIDTH;
		
	for (i = 0; i < chars_to_copy; i++)
	{
	    dest[i] = src[i];
        }
		
	for (i = 0; i < SCREEN_WIDTH; i++)
	{
	    g_console_memory[(SCREEN_HEIGHT - 1) * SCREEN_WIDTH + i] = blank;
	}
		
	vga_move_cursor(g_cursor_x, SCREEN_HEIGHT - 1);
    }
}

void 
put_char(char c)
{
	
    if (c == '\r')
    {
	g_cursor_x = 0;
    }
    else if (c == '\n')
    {
	g_cursor_y++;
	g_cursor_x = 0;
    }
    else if (c == '\b')
    {
        if (g_cursor_x > 0)
	{
	    g_cursor_x--;
	}
    }
    else if (c >= ' ') // isprint(c)
    {
	uint16_t attr = c | (g_text_attr << 8);
	g_console_memory[g_cursor_y * SCREEN_WIDTH + g_cursor_x] = attr; 
	g_cursor_x++;
    }
	
    if (g_cursor_x >= SCREEN_WIDTH)
    {
	g_cursor_x = 0;
	g_cursor_y++;
    }
	
    vga_scroll_screen();
    vga_move_cursor(g_cursor_x, g_cursor_y);
}

void 
put_str(const char *s)
{
    while (*s)
    {
	put_char(*s);
        s++;
    }
}

void print_hex(int h)
{
    char *hex_to_char = "0123456789abcdef";
    char out[] = "0x00000000";

    out[2] = hex_to_char[(h >> 28) & 0xf];
    out[3] = hex_to_char[(h >> 24) & 0xf];

    out[4] = hex_to_char[(h >> 20) & 0xf];
    out[5] = hex_to_char[(h >> 16) & 0xf];

    out[6] = hex_to_char[(h >> 12) & 0xf];
    out[7] = hex_to_char[(h >> 8) & 0xf];

    out[8] = hex_to_char[(h >> 4) & 0xf];
    out[9] = hex_to_char[(h >> 0) & 0xf];

    put_str(out); 
}

/*
 * This printf function can handle:
 *     "%s" strings,
 *     "%x" 32-bit hex values
 *     "%c" ascii characters
 *     "%%"
 *
 * */
void printf(const char *format, ...)
{
    va_list vl;
    va_start(vl, format);

    const char *p = format;
    while (*p)
    {
        if (*p == '%')
	{
            if (p[1] == '%')
	        put_char('%');
            else if (p[1] == 's')
	    {
	        const char *str = va_arg(vl, const char *);
	        put_str(str);	
	    }
	    else if (p[1] == 'x')
	    {
                uint32_t n = va_arg(vl, uint32_t);
		print_hex(n);
	    }
	    else if (p[1] == 'c')
	    {
                uint8_t c = (uint8_t)va_arg(vl, uint32_t);
		put_char(c);
	    }
	    else
	    {
	        put_char(p[1]); 
	    }

	    p += 2; 
	}
	else
	{
            put_char(*p++);	
	}
    }

    va_end(vl); 
}




































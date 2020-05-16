

void vga_move_cursor(int x, int y)
{
    uint16_t temp = y * SCREEN_WIDTH + x;

    outb(0x3d4, 14);
    outb(0x3d5, temp >> 8);
    outb(0x3d4, 15);
    outb(0x3d5, temp);
}

void vga_clear_screen(void)
{
    int len = 2 * 80 * 25;

    volatile uint8_t *video = (volatile uint8_t *)0xb8000;
    while (len-- > 0)
    {
        *video++ = ' ';
	*video++ = 0;
    }
}

void vga_print_message(const char *msg, char attr, int x, int y)
{
    volatile char *video = (volatile char *)0xb8000;

    video += 2 * (y * 80 + x);
    while (*msg)
    {
       *video++ = *msg++;
       *video++ = attr;
    }

}

void vga_print_hex(int h, char attr, int x, int y)
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






































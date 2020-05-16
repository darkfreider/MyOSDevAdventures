

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








































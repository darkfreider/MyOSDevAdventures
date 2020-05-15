






#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25

enum VGA_text_colors
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PINK,
    BROWN,
    GREY,
};
typedef enum VGA_text_colors VGA_text_colors;


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
    int len = 80 * 25;

    volatile short *video = (volatile short *)0xb8000;
    while (len-- > 0)
    {
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








































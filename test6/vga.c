






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








































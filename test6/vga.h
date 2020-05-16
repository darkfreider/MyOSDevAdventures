

#ifndef __VGA_H__
#define __VGA_H__

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

void vga_move_cursor(int x, int y);
void vga_clear_screen(void);
void vga_print_message(const char *msg, char attr, int x, int y);



#endif



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

void vga_set_text_color(uint8_t, uint8_t);
void vga_move_cursor(int, int);
void vga_clear_screen(void);
void vga_scroll_screen(void);

void put_char(char);
void put_str(const char *);
void print_hex(int h);


#endif

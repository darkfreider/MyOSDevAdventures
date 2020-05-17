
#define VK_ESC    0x1b
#define VK_LCTRL  0xa2
#define VK_LSHIFT 0xa0 
#define VK_RSHIFT 0xa1

enum Kbd_dirver_state
{
    KBD_DRIVER_STATE_DEFAULT,
    KBD_DRIVER_STATE_BREAK_1,
    KBD_DRIVER_STATE_BREAK_2,
};
typedef enum Kbd_dirver_state Kbd_dirver_state;


void kbd_handler(void);







#define VK_ESC    0x1b
#define VK_LCTRL  0xa2
#define VK_LSHIFT 0xa0 
#define VK_RSHIFT 0xa1
#define VK_LALT   0x12

#define MSG_KEYUP   0x01
#define MSG_KEYDOWN 0x02

enum Kbd_dirver_state
{
    KBD_DRIVER_STATE_DEFAULT,
    KBD_DRIVER_STATE_BREAK_1,
    KBD_DRIVER_STATE_BREAK_2,
};
typedef enum Kbd_dirver_state Kbd_dirver_state;

struct KBD_msg
{
    uint16_t msg;
    uint16_t vk;

    /*
	(1 << 0) : shift state
	(1 << 1) : ctrl  state
	(1 << 2) : alt   state
    */
    uint16_t flags; 
};
typedef struct KBD_msg KBD_msg;

#define KBD_MSG_QUEUE_SIZE 512
extern KBD_msg g_kbd_msg_queue[KBD_MSG_QUEUE_SIZE];

KBD_msg get_kbd_msg(void);

void kbd_handler(void);






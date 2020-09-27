
char normal_map[256] =
{
    [0x01] = VK_ESC,

    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0a] = '9',
    [0x0b] = '0',
    [0x0c] = '-',
    [0x0d] = '=',
    [0x0e] = '\b',
    [0x0f] = '\t',


    [0x10] = 'Q',
    [0x11] = 'W',
    [0x12] = 'E',
    [0x13] = 'R',
    [0x14] = 'T',
    [0x15] = 'Y',
    [0x16] = 'U',
    [0x17] = 'I',
    [0x18] = 'O',
    [0x19] = 'P',

    [0x1a] = '[',
    [0x1b] = ']',
    [0x1c] = '\n',
    [0x1d] = VK_LCTRL,

    [0x1e] = 'A',
    [0x1f] = 'S',
    [0x20] = 'D',
    [0x21] = 'F',
    [0x22] = 'G',
    [0x23] = 'H',
    [0x24] = 'J',
    [0x25] = 'K',
    [0x26] = 'L',

    [0x27] = ';',
    [0x28] = '\'',
    [0x29] = '`',
    [0x2a] = VK_LSHIFT,
    [0x2b] = '\\',
    
    [0x2c] = 'Z',
    [0x2d] = 'X',
    [0x2e] = 'C',
    [0x2f] = 'V',
    [0x30] = 'B',
    [0x31] = 'N',
    [0x32] = 'M',

    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
    [0x36] = VK_RSHIFT,
    [0x37] = '*',
    [0x38] = VK_LALT, 
    [0x39] = ' ',
};

KBD_msg g_kbd_msg_queue[KBD_MSG_QUEUE_SIZE];
int g_kbd_msg_queue_nitems = 0;
int g_kbd_msg_queue_front  = 0;
int g_kbd_msg_queue_back   = 0;

KBD_msg get_kbd_msg(void)
{
    KBD_msg result;

    int spin = 1;
    do
    {
	cli();
        if (g_kbd_msg_queue_nitems > 0)
	{
            spin = 0;

            result = g_kbd_msg_queue[g_kbd_msg_queue_front++];
	    if (g_kbd_msg_queue_front == KBD_MSG_QUEUE_SIZE)
	    {
	        g_kbd_msg_queue_front = 0; 
	    }
	    g_kbd_msg_queue_nitems--;   
	}
        sti();	
    } while (spin);

    return (result); 
}

// NOTE(max): Keyboard driver puts messages in a message queue
// IMPORTANT(max): This should be used only by kbd driver,
//                 and we assume that interrupts are OFF
void put_kbd_msg(KBD_msg msg)
{
    if (g_kbd_msg_queue_nitems < KBD_MSG_QUEUE_SIZE)
    {
        g_kbd_msg_queue[g_kbd_msg_queue_back++] = msg;
        if (g_kbd_msg_queue_back == KBD_MSG_QUEUE_SIZE)
        {
            g_kbd_msg_queue_back = 0;
        }
	g_kbd_msg_queue_nitems++;
    }
}

Kbd_dirver_state g_kbd_driver_state = KBD_DRIVER_STATE_DEFAULT;

int g_shift_down = 0;
int g_ctrl_down  = 0;
int g_alt_down   = 0;

// IMPORTANT(max): Currently we don't handle Pause key. Don't touch it!
void kbd_handler(void)
{
    uint8_t scan_code = ps2_in_data();
    
    switch (g_kbd_driver_state)
    {
	case KBD_DRIVER_STATE_DEFAULT:
        {
            if (scan_code == 0xe0) // break code
	    {
                g_kbd_driver_state = KBD_DRIVER_STATE_BREAK_1;
	    }
            else
	    {
	        // NOTE(max): normal_map
                KBD_msg msg;

		msg.msg = ((scan_code) & 0x80) ? MSG_KEYUP : MSG_KEYDOWN;
                msg.vk  = normal_map[scan_code & 0x7f];
                if (msg.vk == 0)
		       break;

		switch (msg.vk)
		{
                    case VK_LSHIFT:
	            case VK_RSHIFT:
		    {
		        if (msg.msg == MSG_KEYDOWN)
                            g_shift_down = 1;
		        else
		            g_shift_down = 0;	
		    } break;

		    case VK_LCTRL:
		    {
		        if (msg.msg == MSG_KEYDOWN)
                            g_ctrl_down = 1;
		        else
		            g_ctrl_down = 0;	
		    
		    } break;

		    case VK_LALT:
		    {
		        if (msg.msg == MSG_KEYDOWN)
                            g_alt_down = 1;
		        else
		            g_alt_down = 0;	
		    } break;
		}

	        msg.flags = (g_shift_down << 0) |
		            (g_ctrl_down  << 1) |
		            (g_alt_down   << 2);	    
                
		put_kbd_msg(msg);
	        
		g_kbd_driver_state = KBD_DRIVER_STATE_DEFAULT;	
	    }	    
	} break; 

	case KBD_DRIVER_STATE_BREAK_1:
	{
            if ((scan_code == 0x2a) || (scan_code == 0xb7))
	    {
	        g_kbd_driver_state = KBD_DRIVER_STATE_BREAK_2; 
	    }
            else
	    {
	        // NOTE(max): e0_map
		g_kbd_driver_state = KBD_DRIVER_STATE_DEFAULT;
	    } 	    
	} break;

        case KBD_DRIVER_STATE_BREAK_2:
	{
            if (scan_code == 0x0e)
	    {
		// NOTE(max): 0x0e (0x2a | 0xb7) 0x0e
	        g_kbd_driver_state = KBD_DRIVER_STATE_BREAK_2; 
	    }
	    else
	    {
	        // NOTE(max): 0x0e (0x2a | 0xb7) 0x0e (0x37 | 0xb7) 
		//            print screen pressed/released 
		g_kbd_driver_state = KBD_DRIVER_STATE_DEFAULT;
	    } 
	} break;

        default:
	{
            runtime_assert(0, "Impossible kbd driver state\n");	
	} break;
    } 
}






















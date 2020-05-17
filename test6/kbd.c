
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
    // [0x38] = VK_LALT, don't need this alt key
    [0x39] = ' ',

};

Kbd_dirver_state g_kbd_driver_state = KBD_DRIVER_STATE_DEFAULT;


void kbd_handler(void)
{
    uint8_t scan_code = ps2_in_data();

    print_hex(scan_code);
    put_char('\n');

    switch (g_kbd_driver_state)
    {
	case KBD_DRIVER_STATE_DEFAULT:
        {
            if (scan_code == 0x0e) // break code
	    {
                g_kbd_driver_state = KBD_DRIVER_STATE_BREAK_1;
	    }
            else
	    {
	        // NOTE(max): normal_map
		if ((scan_code & 0x80) == 0)
		{
		    /*uint8_t vkey = normal_map[scan_code];
		    if (vkey >= 0x20 && vkey <= 0x7e)
		    {
                        put_char(vkey); 
		    }*/
		    //print_hex(scan_code);	
	        }

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






















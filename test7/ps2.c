

uint8_t
ps2_in_data(void)
{
    return inb(PS2_DATA);
}

uint8_t
ps2_in_data_wait(void)
{
    while ( !(inb(PS2_STATUS) & PS2_OUT_BUF_STATUS) )
	    ;

    return inb(PS2_DATA);
}

void
ps2_out_data_wait(uint16_t port, uint8_t data)
{
    runtime_assert((port == PS2_DATA) || (port == PS2_CMD), 
		    "ps2_out_data_wait failed");

    while ((inb(PS2_STATUS) & PS2_IN_BUF_STATUS))
	    ;
    
    outb(port, data);
}

// NOTE(max): I initialize ps2 controller for keyboard use only.
void 
ps2_controller_init(void)
{
    // NOTE(max): disable devices 
    ps2_out_data_wait(PS2_CMD, PS2_CMD_DISABLE_PORT_1);
    ps2_out_data_wait(PS2_CMD, PS2_CMD_DISABLE_PORT_2);

    // NOTE(max): flush output buffer
    ps2_in_data();

    // NOTE(max): set configuration byte
    ps2_out_data_wait(PS2_CMD, PS2_CMD_READ_CONFIG_BYTE);
    uint8_t config_byte = ps2_in_data_wait();

    // NOTE(max): disable interrups from a mouse
    config_byte &= ~(PS2_CONFIG_PORT_2_INT);
    // NOTE(max): enable scan code translation (we'll get scanset 1)
    //            and enable interrups 
    config_byte |= (PS2_CONFIG_PORT_1_INT |
		    PS2_CONFIG_PORT_1_TRANSLATION);
    
    ps2_out_data_wait(PS2_CMD, PS2_CMD_WRITE_CONFIG_BYTE);
    ps2_out_data_wait(PS2_DATA, config_byte);

    // NOTE(max): Reset keyboard
    //            Keyboard will send 2 bytes:
    //            0xfa - command acknowledge
    //            0xaa - self test passed (sent after reset(0xff) command)
    ps2_out_data_wait(PS2_DATA, PS2_KBD_CMD_RESET);
#if 1
    ps2_in_data_wait(); // 0xfa - cmd akw
    ps2_in_data_wait(); // 0xaa - self test passed
#else 
    uint8_t val = ps2_in_data_wait();
    put_str("sp2 end init: ");
    print_hex(val);
    put_char('\n');
    
    val = ps2_in_data_wait();
    put_str("sp2 end init: ");
    print_hex(val);
    put_char('\n');
#endif

    // NOTE(max): enable keyboard
    ps2_out_data_wait(PS2_CMD, PS2_CMD_ENABLE_PORT_1);

}













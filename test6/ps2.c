

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
    ps2_in_data_wait();

    // NOTE(max): set configuration byte
    



}













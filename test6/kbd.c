


#define PS2_DATA    0x60
#define PS2_STATUS  0x64
#define PS2_COMMAND 0x64

#define PS2_OUT_BUF_STATUS (1 << 0)
#define PS2_IN_BUF_STATUS  (1 << 1) 

#define PS2_CMD_READ_CONFIG_BYTE 0x20
#define PS2_CMD_WRITE_CONFIG_BYTE 0x60

uint8_t
ps2_in_data(void)
{
	return inb(PS2_DATA);
}

// NOTE(max): I initialize ps2 controller for keyboard use only.
void 
ps2_controller_init(void)
{

}














#ifndef __PS2_H__
#define __PS2_H__

#define PS2_DATA    0x60
#define PS2_STATUS  0x64
#define PS2_CMD     0x64

#define PS2_OUT_BUF_STATUS (1 << 0)
#define PS2_IN_BUF_STATUS  (1 << 1) 

#define PS2_CONFIG_PORT_1_INT         (1 << 0)
#define PS2_CONFIG_PORT_2_INT         (1 << 1)
#define PS2_CONFIG_PORT_1_TRANSLATION (1 << 6)

#define PS2_CMD_READ_CONFIG_BYTE  0x20
#define PS2_CMD_WRITE_CONFIG_BYTE 0x60
#define PS2_CMD_DISABLE_PORT_1    0xad
#define PS2_CMD_DISABLE_PORT_2    0xa7 
#define PS2_CMD_ENABLE_PORT_1     0xae 

#define PS2_KBD_CMD_RESET 0xff

uint8_t ps2_in_data(void);
uint8_t ps2_in_data_wait(void);
void ps2_out_data_wait(uint16_t, uint8_t);
void ps2_controller_init(void);


#endif











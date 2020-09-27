
#include <stdint.h>
#include <stdarg.h>

#include "../trap.h"
#include "../vga.h"

#include "../x86.c"
#include "../vga.c"


uint8_t rule22[] = { 0, 0, 0, 1, 0, 1, 1, 0 };
uint8_t buf_0[80 + 2];
uint8_t buf_1[80 + 2];

uint8_t get_cell_state(uint8_t left, uint8_t me, uint8_t right)
{
    uint8_t state = (left << 2) + (me << 1) + (right << 0); 
    return rule22[7 - state];
}

int main(void)
{
    uint32_t count = 80 * 5;

    for (int i = 0; i < 82; i++)
    {
        buf_0[i] = (i % 2) ? 1 : 0;
    }
 
    uint8_t *curr_state = buf_0;
    uint8_t *next_state = buf_1;
    while (count --> 0)
    {
        for (uint32_t i = 1; i < 81; i++)
	{
            next_state[i] = get_cell_state(curr_state[i - 1],
		                           curr_state[i],
			                   curr_state[i + 1]);		   
	}

        for (uint32_t i = 0; i < 80; i++)
	{
            put_char(curr_state[i + 1] ? '1' : ' ');	
	}

        uint8_t *temp = curr_state;
        curr_state = next_state;
        next_state = temp;

        for (uint32_t i = 0; i < 4000000; i++);	
        for (uint32_t i = 0; i < 4000000; i++);	
        for (uint32_t i = 0; i < 4000000; i++);	
    }

    return (0);
}




















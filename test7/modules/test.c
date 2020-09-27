

#include <stdint.h>
#include <stdarg.h>

extern void syscall(uint32_t num, void *param);

void
put_str(const char *s)
{
    syscall(7, (void *)s);
}

void put_char(char c)
{
    char buf[2];
    buf[0] = c;
    buf[1] = 0;

    syscall(7, buf);
}

void print_hex(int h)
{
    char *hex_to_char = "0123456789abcdef";
    char out[] = "0x00000000";

    out[2] = hex_to_char[(h >> 28) & 0xf];
    out[3] = hex_to_char[(h >> 24) & 0xf];

    out[4] = hex_to_char[(h >> 20) & 0xf];
    out[5] = hex_to_char[(h >> 16) & 0xf];

    out[6] = hex_to_char[(h >> 12) & 0xf];
    out[7] = hex_to_char[(h >> 8) & 0xf];

    out[8] = hex_to_char[(h >> 4) & 0xf];
    out[9] = hex_to_char[(h >> 0) & 0xf];

    put_str(out); 
}

/*
 * This printf function can handle:
 *     "%s" strings,
 *     "%x" 32-bit hex values
 *     "%c" ascii characters
 *     "%%"
 *
 * */
void printf(const char *format, ...)
{
    va_list vl;
    va_start(vl, format);

    const char *p = format;
    while (*p)
    {
        if (*p == '%')
        {
            if (p[1] == '%')
                put_char('%');
            else if (p[1] == 's')
            {
                const char *str = va_arg(vl, const char *);
                put_str(str);
            }
            else if (p[1] == 'x')
            {
                uint32_t n = va_arg(vl, uint32_t);
                print_hex(n);
            }
            else if (p[1] == 'c')
            {
                uint8_t c = (uint8_t)va_arg(vl, uint32_t);
                put_char(c);
            }
            else
            {
                put_char(p[1]);
            }

            p += 2;
        }
        else
        {
            put_char(*p++);
        }
    }

    va_end(vl);
}


int main(void)
{
    printf("Hello from test module!\n");
    put_str("0xdeadbeef\n");
    put_char('\n');

    syscall(42, 0);

    return (0);
}
















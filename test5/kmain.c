
#if defined(__linux__)
#error "Your are not using cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This should be compiled with ix86 compiler"
#endif

char g_attr = 0x07;

void print_message(const char *msg, char attr)
{
    volatile char *video = (volatile char *)0xb8000; 
    while (*msg)
    {
       *video++ = *msg++;
       *video++ = attr; 
    } 

}


int _start(void)
{
char *msg[] = {
    "                     works!",
    "            c kernel ",
    "      world ",
    "hello ", 

};

    for (int i = 0; i < 4; i++)
	    print_message(msg[i], 0x07);

    for (;;);
    return (0);
}

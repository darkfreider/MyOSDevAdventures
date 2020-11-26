
#if defined(__linux__)
#error "Your are not using cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This should be compiled with ix86 compiler"
#endif

#define static_assert(e) do { switch(0) { case 0: case (e):break; } } while(0)

#include <stdint.h>
#include <stdarg.h>

#include "vga.h"

void runtime_assert(int e, const char *msg)
{
    if (e == 0)
    {
        vga_clear_screen();
        put_str(msg);
    }
}

#include "x86.c"
#include "vga.c"

typedef struct TCB
{
    uint32_t *stack;
} TCB;

#define STACK_SIZE (256 / sizeof(uint32_t))
uint32_t free_stacks[4][STACK_SIZE];
TCB test_tcbs[2];

typedef void (*Thread_proc_t)(void);
extern void thread_init_stack_frame(Thread_proc_t, uint32_t **);
extern void thread_switch(uint32_t **old_sp, uint32_t **new_sp);
extern void sheduler_start(uint32_t *sp);

void t0(void)
{
    for (;;)
    {
        printf("Hello from t0!\n");
        for (volatile uint32_t i = 0; i < 300000000; i++)
            ;
        thread_switch(&test_tcbs[0].stack, &test_tcbs[1].stack);
    }
}

void t1(void)
{
    for (;;)
    {
        printf("thread 1 is here!\n");
        for (volatile uint32_t i = 0; i < 300000000; i++)
            ;
        thread_switch(&test_tcbs[1].stack, &test_tcbs[0].stack);
    }
}


void
kmain(uint32_t magic)
{
    vga_set_text_color(BLACK, GREY);
    vga_clear_screen();

    printf("Hello from kernel!\n");

    test_tcbs[0].stack = &free_stacks[0][0];
    thread_init_stack_frame(t0, &test_tcbs[0].stack);

    test_tcbs[1].stack = &free_stacks[1][0];
    thread_init_stack_frame(t1, &test_tcbs[1].stack);

    sheduler_start(test_tcbs[0].stack);

    for(;;);
}

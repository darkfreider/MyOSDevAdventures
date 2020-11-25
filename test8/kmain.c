
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
uint32_t g_tcb_arr_free_slot;
TCB g_tcb_arr[4];
uint32_t free_stacks[4][STACK_SIZE];

void *g_thread_handlers_arr[4] =
{
    (void *)0xffffffff,
    (void *)0xffffffff,
    (void *)0xffffffff,
    (void *)0xffffffff,
};
uint32_t g_thread_id;

typedef void (*Thread_proc_t)(void);
extern void thread_init_stack_frame(Thread_proc_t, uint32_t *);
extern void thread_switch(uint32_t **old_sp, uint32_t **new_sp);
extern void sheduler_start(uint32_t *sp);

TCB *thread_create(Thread_proc_t thread_proc)
{
    int new_thread_id = 0;
    for (new_thread_id = 0; new_thread_id < 4; new_thread_id++)
    {
        if (g_thread_handlers_arr[new_thread_id] == (void *)0xffffffff)
            break;
    }

    uint32_t slot_index = g_tcb_arr_free_slot++;

    TCB *new_tcb = &g_tcb_arr[slot_index];
    g_thread_handlers_arr[new_thread_id] = (void *)new_tcb;

    new_tcb->stack = &free_stacks[slot_index][0];

    thread_init_stack_frame(thread_proc, new_tcb->stack);

    return new_tcb;
}

void thread_yield(void)
{
    TCB *old_tcb = &g_tcb_arr[g_thread_id];

    g_thread_id = (g_thread_id + 1) % 4;
    while (g_thread_handlers_arr[g_thread_id] == (void *)0xffffffff)
    {
        g_thread_id = (g_thread_id + 1) % 4;
    }

    TCB *next_tcb = &g_tcb_arr[g_thread_id];

    thread_switch(&old_tcb->stack, &next_tcb->stack);
}

void t0(void)
{
    for (;;)
    {
        printf("Hello from t0!\n");
        for (volatile uint32_t i = 0; i < 1000500000; i++)
            ;

        thread_yield();
    }
}

void t1(void)
{
    for (;;)
    {
        printf("Hello from t0!\n");
        for (volatile uint32_t i = 0; i < 1000500000; i++)
            ;

        thread_yield();
    }
}

void
kmain(uint32_t magic)
{
    vga_set_text_color(BLACK, GREY);
    vga_clear_screen();

    printf("Hello from kernel!\n");

    TCB *tcb = 0;

    tcb = thread_create(t0);
    printf("stack addr: %x\n", tcb->stack);
    //tcb = thread_create(t1);
    //printf("stack addr: %x\n", tcb->stack);

    printf("t0 addr: %x\n", t0);
    for (int i = 0; i < 10; i++)
    {
        printf("%x : %x\n", &tcb->stack[i], tcb->stack[i]);
    }

    //for (volatile uint32_t i = 0; i < 1000500000; i++);

#if 0
    {
        TCB *first_thread = 0;

        int i = 0;
        for (i = 0; i < 4; i++)
        {
            if (g_thread_handlers_arr[i] != (void *)0xffffffff)
            {
                first_thread = (TCB *)&g_tcb_arr[i];
                break;
            }
        }

        if (first_thread)
        {
            g_thread_id = i;
            sheduler_start(first_thread->stack);
        }
    }
#endif
    for(;;);
}

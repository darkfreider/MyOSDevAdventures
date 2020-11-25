
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
    uint32_t id;
    uint32_t *stack;
    void *sp;
} TCB;

TCB *g_current_tcb;

#define STACK_SIZE (256 / sizeof(uint32_t))
uint32_t free_stacks[STACK_SIZE][2];
TCB      free_tcb_array[2];
int32_t tcb_index = 0;

TCB *
thread_create(void (*thread_proc)(void *))
{
    TCB *result = &free_tcb_array[0];
   
    result->id = 1; 
    result->stack = (uint32_t *)&free_stacks[0][0] + STACK_SIZE; 

    return (result);
}

void thread_yield(void)
{
}

void start_scheduler(void)
{

}

void 
kmain(uint32_t magic)
{
    vga_set_text_color(BLACK, GREY);  
    vga_clear_screen();
    
    printf("Hello from kernel!\n");

    TCB *tcb = thread_create(0);
    printf("tcb addr: %x\n", tcb);
    printf("stack addr: %x\n", tcb->stack);

#if 0
    thread_create(thread1);
    thread_create(thread2);

    start_scheduler();
#endif

    for(;;);
}














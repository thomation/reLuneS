#include "os.h"

extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);
void start_kernel()
{
    uart_init();
    uart_puts("Hello reLune\n");
    page_init();
    uart_puts("Init Sched\n");
    sched_init();
    schedule();
    uart_puts("Would not go here!\n");

    while(1) {}
}
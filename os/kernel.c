#include "os.h"

extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);
extern void os_main(void);
extern void trap_init(void);
extern void plic_init(void);
extern void timer_init(void);

void start_kernel()
{
    uart_init();
    uart_puts("Hello reLune\n");
    page_init();
    trap_init();
    plic_init();
    timer_init();

    uart_puts("Init Sched\n");
    sched_init();
    os_main();
    schedule();
    uart_puts("Would not go here!\n");

    while(1) {}
}
#include "os.h"

extern void uart_init(void);
extern void page_init(void);

void start_kernel()
{
    uart_init();
    uart_puts("Hello reLune\n");
    page_init();

    while(1) {}
}
#include "os.h"

extern void uart_init(void);

void start_kernel()
{
    uart_init();
    uart_puts("Hello reLune\n");
    while(1) {}
}
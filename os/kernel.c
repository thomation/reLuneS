extern void uart_init(void);
extern void uart_puts(char *s);

void start_kernel()
{
    uart_init();
    uart_puts("Hello reLune\n");
    while(1) {}
}
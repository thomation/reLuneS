#include "os.h"

extern void trap_vector(void);
extern void uart_isr(void);
extern void timer_handler(void);
extern void schedule(void);
extern void do_syscall(struct context *cxt);

void trap_init()
{
	/*
	 * set the trap-vector base-address for machine-mode
	 */
	w_mtvec((reg_t)trap_vector);
}
void external_interrupt_handler()
{
	int irq = plic_claim();

	if (irq == UART0_IRQ)
	{
		uart_isr();
	}
	else if (irq)
	{
		printf("unexpected interrupt irq = %d\n", irq);
	}

	if (irq)
	{
		plic_complete(irq);
	}
}

reg_t trap_handler(reg_t epc, reg_t cause, struct context *cxt)
{
	reg_t return_pc = epc;
	reg_t cause_code = cause & 0xfff;

	if (cause & 0x80000000)
	{
		/* Asynchronous trap - interrupt */
		switch (cause_code)
		{
		case 3:
			uart_puts("software interruption!\n");
			int id = r_mhartid();
			*(uint32_t *)CLINT_MSIP(id) = 0;
			schedule();
			break;
		case 7:
			uart_puts("timer interruption!\n");
			timer_handler();
			break;
		case 11:
			uart_puts("external interruption!\n");
			external_interrupt_handler();
			break;
		default:
			uart_puts("unknown async exception!\n");
			break;
		}
	}
	else
	{
		switch (cause_code)
		{
		case 8:
			uart_puts("system call from u mode!\n");
			do_syscall(cxt);
			return_pc += 4;
			break;
		default:
			/* Synchronous trap - exception */
			printf("Sync exceptions!, code = %d\n", cause_code);
			panic("OOPS! What can I do!");
			// return_pc += 4;
			break;
		}
	}

	return return_pc;
}

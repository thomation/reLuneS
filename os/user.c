#include "os.h"

#define DELAY 1000

void idle(void *param)
{
    printf("Idle Task: Created\n");
    while (1)
    {
        uart_puts("Idle Task: Running...\n");
        task_delay(DELAY);
        task_yield();
    }
}
void user_task0(void *param)
{
    printf("Task 0: Created! param: %d\n", param);
    task_yield();
    printf("Task 0: I'm back param: %d\n", param);
    int times = (int)param;
    while (1)
    {
        spin_lock();
        for (int i = 0; i < times; i++)
        {
            printf("Task 0: Running... %d \n", i);
            task_delay(DELAY);
        }
        spin_unlock();
    }
}

void user_task1(void *param)
{
    uart_puts("Task 1: Created!\n");
    uart_puts("Task 1: get hart id!\n");
    int hid = r_mhartid();
	printf("hart id is %d\n", hid);
    int times = (int)param;
    for (int i = 0; i < times; i++)
    {
        printf("Task 1: Running count:%d...\n", i);
        task_delay(DELAY);
        task_yield();
    }
    uart_puts("Task 1: exit!\n");
    task_exit();
}

void user_task2(void *param)
{
    uart_puts("Task 2: Created!\n");
    int times = (int)param;
    while (1)
    {
        spin_lock();
        for (int i = 0; i < times; i++)
        {
            printf("Task 2: Running... %d \n", i);
            task_delay(DELAY);
        }
        spin_unlock();
        // task_yield();
        // *(int *)0x00000000 = 100; // Cause exception
    }
}
/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void *param)
{
    task_create(idle, NULL, 255);
    task_create(user_task0, (void *)5, 10);
    task_create(user_task1, (void*)10, 5);
    task_create(user_task2, (void *)5, 10);
}
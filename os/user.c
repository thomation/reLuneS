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
    while (1)
    {
        uart_puts("Task 0: Running...\n");
        task_delay(DELAY);
        task_yield();
    }
}

void user_task1(void *param)
{
    uart_puts("Task 1: Created!\n");
    int times = param;
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
    while (1)
    {
        uart_puts("Task 2: Running...\n");
        task_delay(DELAY);
        task_yield();
        *(int *)0x00000000 = 100; // Cause exception
    }
}
/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void *param)
{
    int times = 10;
    task_create(idle, NULL, 255);
    task_create(user_task0, times, 10);
    task_create(user_task1, times, 5);
    task_create(user_task2, times, 10);
}
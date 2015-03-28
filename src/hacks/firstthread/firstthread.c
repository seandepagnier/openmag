#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 8000000UL
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#define BOOTLOADER_OFFSET 0xf800
#include <uart.h>

#define NUM_THREADS 2
#include <threads.h>

// threads //
void thread1(void)
{
    static int i=0;
    for(;;) {
        printf("thread1 %d\n", i++);
        _delay_ms(10);
        thread_relinquish();
    }
}

void thread2(void)
{
    static int i;
    for(;;) {
        printf("thread2 %d %d\n", i+=2);
        _delay_ms(10);
        thread_relinquish();
    }
}

thread threads[NUM_THREADS] = {thread1, thread2};

int main(void)
{
    uart_init(12);
    sei();

    printf("starting threads...\n");

    threads_start();

    printf("all threads exited\n");
    return 0;
}

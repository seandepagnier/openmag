#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2
#include <threads.h>

// threads //
void thread1(void)
{
    static int i=0;
    for(;;) {
        printf("thread1 %d\n", i++);
        thread_relinquish();
    }
}

void thread2(void)
{
    static int i;
    for(;;) {
        printf("thread2 %d\n", i+=2);
        thread_relinquish();
    }
}

thread threads[NUM_THREADS] = {thread1, thread2};

int main(void)
{
    printf("starting threads...\n");

    threads_start();

    printf("all threads exited\n");
}

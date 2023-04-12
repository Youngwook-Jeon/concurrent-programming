#include <pthread.h>
#include <stdio.h>

void barrier(volatile int *cnt, int max)
{
    __sync_fetch_and_add(cnt, 1);
    while (*cnt < max)
        ;
}

volatile int num = 0; // 공유 변수

void *worker(void *arg)
{
    barrier(&num, 10); // 모든 스레드가 여기에 도달할 때까지 대기
    // 비지니스 로직

    return NULL;
}

int main(int argc, char *argv[])
{
    // 스레드 생성
    pthread_t th[10];
    for (int i = 0; i < 10; i++)
    {
        if (pthread_create(&th[i], NULL, worker, NULL) != 0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    // join은 생략
    return 0;
}
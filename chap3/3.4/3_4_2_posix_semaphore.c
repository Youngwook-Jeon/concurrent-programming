#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10
#define NUM_LOOP 10 // 스레드 안의 루프 수

int count = 0;

void *th(void *arg)
{
    // 이름있는 세마포어를 엶.
    sem_t *s = sem_open("/mysemaphore", 0);
    if (s == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    for (int i = 0; i < NUM_LOOP; i++)
    {
        // 대기
        if (sem_wait(s) == -1)
        {
            perror("sem_wait");
            exit(1);
        }

        __sync_fetch_and_add(&count, 1);
        printf("count = %d\n", count);

        // 10ms 슬립
        usleep(10000);

        __sync_fetch_and_sub(&count, 1);

        // 세마포 값을 증가, 크리티컬 섹션에서 벗어남.
        if (sem_post(s) == -1)
        {
            perror("sem_post");
            exit(-1);
        }
    }

    if (sem_close(s) == -1)
        perror("sem_close");

    return NULL;
}

int main(int argc, char *argv[])
{
    // 이름이 붙은 세마보를 연다. 세마포가 없을 때는 생성한다.
    // 자신과 그룹을 이용할 수 있는 세미포로,
    // 크리티컬 섹션에 들어갈 수 있는 프로세스는 최대 3개이다.
    sem_t *s = sem_open("/mysemaphore", O_CREAT, 0660, 3);
    if (s == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }

    // 스레드 생성
    pthread_t v[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&v[i], NULL, th, NULL);
    }

    // join
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(v[i], NULL);
    }

    // 세마포를 닫는다
    if (sem_close(s) == -1)
        perror("sem_close");

    // 세마포 파기
    if (sem_unlink("/mysemaphore") == -1)
        perror("sem_unlink");

    return 0;
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t barrier_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_cond = PTHREAD_COND_INITIALIZER;

void barrier(volatile int *cnt, int max)
{
    if (pthread_mutex_lock(&barrier_mut) != 0)
    {
        perror("pthread_mutex_lock");
        exit(-1);
    }

    (*cnt)++; // 락을 획득하고 난 후 공유 변수를 증가시킴

    if (*cnt == max)
    {
        // 모든 프로세스가 모였으므로 알림
        if (pthread_cond_broadcast(&barrier_cond) != 0)
        {
            perror("pthread_cond_broadcast");
            exit(-1);
        }
    }
    else
    {
        do
        { // 모든 프로세스가 모일 때까지 대기
            if (pthread_cond_wait(&barrier_cond,
                                  &barrier_mut) != 0)
            {
                perror("pthread_cond_wait");
                exit(-1);
            }
        } while (*cnt < max); // 의사 각성을 위한 조건
    }

    if (pthread_mutex_unlock(&barrier_mut) != 0)
    {
        perror("pthread_mutex_unlock");
        exit(-1);
    }
}

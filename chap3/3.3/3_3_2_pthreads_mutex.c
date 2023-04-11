#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER; // 뮤텍스용 변수 mut 정의. 초기화는 PTHREAD_MUTEX_INITIALIZER 매크로가 수행.

void *some_func(void *arg)
{
    if (pthread_mutex_lock(&mut) != 0)
    {
        perror("pthread_mutext_lock");
        exit(-1);
    }

    // 크리티컬 섹션

    if (pthread_mutex_unlock(&mut) != 0)
    {
        perror("pthread_mutex_unlock");
        exit(-1);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    // 스레드 생성
    pthread_t th1, th2;
    if (pthread_create(&th1, NULL, some_func, NULL) != 0)
    {
        perror("pthread_create");
        return -1;
    }

    if (pthread_create(&th2, NULL, some_func, NULL) != 0)
    {
        perror("pthread_create");
        return -1;
    }

    // 스레드 종료 대기
    if (pthread_join(th1, NULL) != 0)
    {
        perror("pthread_join");
        return -1;
    }

    if (pthread_join(th2, NULL) != 0)
    {
        perror("pthread_join");
        return -1;
    }

    // 뮤텍스 객체 반환. pthread_mutex_destroy 함수로 반환하지 않으면 메모리 릭 발생.
    if (pthread_mutex_destroy(&mut) != 0)
    {
        perror("pthread_mutex_destroy");
        return -1;
    }

    return 0;
}

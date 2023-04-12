#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

volatile bool ready = false;
char buf[256]; // 스레드 사이에서 데이터를 주고 받기 위한 버퍼

void *producer(void *arg)
{ // 프로듀서 스레드
    printf("producer: ");
    fgets(buf, sizeof(buf), stdin);

    pthread_mutex_lock(&mut);
    ready = true;

    if (pthread_cond_broadcast(&cond) != 0)
    { // 컨디션 변수를 기다리는 모든 스레드를 깨움
        perror("pthread_cond_broadcast");
        exit(-1);
    }

    pthread_mutex_unlock(&mut);
    return NULL;
}

void *consumer(void *arg)
{ // 컨슈머 스레드
    pthread_mutex_lock(&mut);

    while (!ready)
    { // ready 변수값이 false인 경우 대기
        // 록 반환과 대기를 동시에 실행
        // 대기 중 다른 스레드가 pthread_cond_broadcast or pthread_cond_signal 함수로 알림을 하면 대기를 종료하고 다시 락을 획득해서 처리 재개
        if (pthread_cond_wait(&cond, &mut) != 0)
        {
            perror("pthread_cond_wait");
            exit(-1);
        }
    }

    pthread_mutex_unlock(&mut);
    printf("consumer: %s\n", buf);
    return NULL;
}

int main(int argc, char *argv[])
{
    // 스레드 생성
    pthread_t pr, cn;
    pthread_create(&pr, NULL, producer, NULL);
    pthread_create(&cn, NULL, consumer, NULL);

    // 스레드 종료 대기
    pthread_join(pr, NULL);
    pthread_join(cn, NULL);

    // 뮤텍스 객체 반환
    pthread_mutex_destroy(&mut);

    // 조건 변수 객체 반환
    if (pthread_cond_destroy(&cond) != 0)
    {
        perror("pthread_cond_destroy");
        return -1;
    }

    return 0;
}
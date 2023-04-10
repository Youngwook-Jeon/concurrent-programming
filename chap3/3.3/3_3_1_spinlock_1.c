#include "../3.2/3_2_2_tas.c"

// 정상적인 코드지만 실행 속도 패널티가 큰 아토믹 명령을 지속적으로 사용.
void spinlock_acquire(bool *lock)
{
    while (test_and_set(lock))
        ;
}

void spinlock_release(bool *lock)
{
    tas_release(lock);
}
#include "../3.2/3_2_2_tas.c"

// TAS 호출전 검사를 먼저 하고 TAS를 수행하도록 개선.
void spinlock_acquire(volatile bool *lock)
{
    for (;;)
    {
        while (*lock)
            ;
        if (!test_and_set(lock))
            break;
    }
}

void spinlock_release(bool *lock)
{
    tas_release(lock);
}
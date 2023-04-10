#include <stdbool.h>

// 입력된 포인터가 가리키는 값이 true => true 반환, false이면 포인터가 가리키는 메모리 값을 true로 설정하고 false 반환.
bool test_and_set(volatile bool *p)
{
    return __sync_lock_test_and_set(p, 1);
}

void tas_release(volatile bool *p)
{
    return __sync_lock_release(p);
}
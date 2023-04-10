#include <stdint.h>
#include <stdbool.h>

// 내장 함수를 사용하여 아토믹으로 처리.
// lock cmpxchgq	%rdx, (%rdi) 어셈블리 문장을 통해 지정된 명령중의 메모리 접근을 배타적으로 수행됨을 보장.
bool compare_and_swap(uint64_t *p, uint64_t val, uint64_t newval)
{
    return __sync_bool_compare_and_swap(p, val, newval);
}
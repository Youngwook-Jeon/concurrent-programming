#include <stdint.h>
#include <stdbool.h>

// CAS의 의미를 나타내기 위한 코드로, 아토믹한 코드는 아님
bool compare_and_swap(uint64_t *p, uint64_t val, uint64_t newval)
{
    if (*p != val)
    {
        return false;
    }
    *p = newval;
    return true;
}
#include <stdbool.h>

bool lock = false;

void bad_mutex()
{
retry:
    if (!lock)
    {
        lock = true;
        // 크리티컬 섹션
    }
    else
    {
        goto retry;
    }
    lock = false;
}
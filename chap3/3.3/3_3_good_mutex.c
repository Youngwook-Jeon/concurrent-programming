#include "../3.2/3_2_2_tas.c"

bool lock = false;

void good_mutex()
{
retry:
    if (!test_and_set(&lock))
    {
        // 크리티컬 섹션
    }
    else
    {
        goto retry;
    }
    tas_release(&lock);
}
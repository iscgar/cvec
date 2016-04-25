#include <stdio.h>
#include <stdlib.h>
#include "cvec.h"

using_vec_type(int);

int cmp(const int *a, const int *b)
{
    if (*a > *b)
    {
        return 1;
    }

    if (*a == *b)
    {
        return 0;
    }

    return -1;
}

int main()
{
    int result = EXIT_FAILURE;

    vec_type(int) test_vec;

    puts("Hello, world!");

    if (!vec_init(int)(&test_vec))
    {
        puts("FAIL: vec_init");
    }
    else
    {
        unsigned int i = 0;
        int success = 1, prev = 0;

        for (i = 1; success && i <= 10; ++i)
        {
            success = vec_push(int)(&test_vec, i);
        }

        if (!success)
        {
            puts("FAIL: vec_push");
            goto done;
        }

        for (i = 0; success && i < 3; ++i)
        {
            success = vec_unshift(int)(&test_vec, NULL);
        }

        if (!success)
        {
            puts("FAIL: vec_unshift");
            goto done;
        }

        for (i = 13; success && i >= 11; --i)
        {
            success = vec_push(int)(&test_vec, i);
        }

        if (!success)
        {
            puts("FAIL: second vec_push");
            goto done;
        }

        for (i = 3; success && i < 5; ++i)
        {
            int j = 0;
            success = vec_erase(int)(&test_vec, i, 1, &j);
        }

        if (!success)
        {
            puts("FAIL: vec_erase");
            goto done;
        }

        for (i = 0; success && i < 3; ++i)
        {
            success = vec_unshift(int)(&test_vec, NULL);
        }

        if (!success)
        {
            puts("FAIL: second vec_unshift");
            goto done;
        }

        prev = *vec_get(int)(&test_vec, 0);
        printf("Before sort: [%d", prev);
        for (i = 1; i < vec_size(int)(&test_vec); ++i)
        {
            int current = *vec_get(int)(&test_vec, i);
            printf(", %d", current);
            success = success || prev > current;
            prev = current;
        }
        puts("]");

        if (!success)
        {
            puts("FAIL: vector is already sorted");
            goto done;
        }

        vec_sort(int)(&test_vec, cmp);

        prev = *vec_get(int)(&test_vec, 0);
        printf("After sort: [%d", prev);
        for (i = 1; i < vec_size(int)(&test_vec); ++i)
        {
            int current = *vec_get(int)(&test_vec, i);
            printf(", %d", current);
            success = success && prev <= current;
            prev = current;
        }
        puts("]");

        if (!success)
        {
            puts("ERROR: vec_sort failed");
            goto done;
        }

        result = EXIT_SUCCESS;

done:
        vec_clear(int)(&test_vec);

        puts("Done.");
    }

    return result;
}

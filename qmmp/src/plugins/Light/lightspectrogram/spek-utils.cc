#include <assert.h>
#include <stdlib.h>

#include "spek-utils.h"

int spek_vercmp(const char *a, const char *b)
{
    assert(a && b);

    if (!*a && !*b) {
        return 0;
    }
    if (!*a) {
        return -1;
    }
    if (!*b) {
        return 1;
    }

    char *i, *j;
    while (true) {
        i = j = NULL;
        const long x = strtol(a, &i, 10);
        const long y = strtol(b, &j, 10);

        if (x < y) {
            return -1;
        }
        if (x > y) {
            return 1;
        }

        if (!*i && !*j) {
            return 0;
        }
        if (!*i) {
            return -1;
        }
        if (!*j) {
            return 1;
        }

        a = i + 1;
        b = j + 1;
    }
}

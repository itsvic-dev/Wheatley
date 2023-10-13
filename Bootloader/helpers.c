#include "helpers.h"

bool memcmp(void *a, void *b, int n) {
    for (int i = 0; i < n; i++) {
        if (((uint_8b *)a)[i] != ((uint_8b *)b)[i]) return false;
    }
    return true;
}

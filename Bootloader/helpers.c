#include "helpers.h"

bool memcmp(void *a, void *b, int n) {
    for (int i = 0; i < n; i++) {
        if (((uint8_t *)a)[i] != ((uint8_t *)b)[i]) return false;
    }
    return true;
}

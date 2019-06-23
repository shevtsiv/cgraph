#include <stdlib.h>

bool arraysCompare(const size_t firstArray[], const size_t secondArray[], size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (firstArray[i] != secondArray[i]) {
            return false;
        }
    }
    return true;
}

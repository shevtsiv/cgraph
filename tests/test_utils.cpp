#include <stdlib.h>

bool arraysCompare(const int firstArray[], const int secondArray[], size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (firstArray[i] != secondArray[i]) {
            return false;
        }
    }
    return true;
}

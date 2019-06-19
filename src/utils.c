int checkAllVisited(const int visitedNodes[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (visitedNodes[i] == 0) {
            return 0;
        }
    }
    return 1;
}

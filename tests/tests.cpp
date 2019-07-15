#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/graph.c"
#include "test_utils.cpp"

// Stack tests =========================

TEST(ListTest, CreateListTest) {
    Stack *stack = createStack();
    ASSERT_NE(stack, nullptr);
    ASSERT_EQ(stack->head, nullptr);
    free(stack);
}

TEST(ListTest, AddToList) {
    Stack *stack = createStack();
    ASSERT_EQ(stack->head, nullptr);
    ASSERT_EQ(stack->size, 0);
    pushToStack(stack, 1);
    ASSERT_NE(stack->head, nullptr);
    ASSERT_EQ(stack->head->data, 1);
    ASSERT_EQ(stack->size, 1);
    pushToStack(stack, 2);
    ASSERT_EQ(stack->head->data, 1);
    ASSERT_EQ(stack->size, 2);
    pushToStack(stack, 5);
    ASSERT_EQ(stack->head->data, 1);
    ASSERT_EQ(stack->size, 3);
    freeStack(stack);
}

TEST(ListTest, PopFromList) {
    Stack *stack = createStack();
    ASSERT_EQ(stack->head, nullptr);
    ASSERT_EQ(stack->size, 0);
    pushToStack(stack, 1);
    ASSERT_NE(stack->head, nullptr);
    ASSERT_EQ(stack->head->data, 1);
    ASSERT_EQ(stack->size, 1);
    pushToStack(stack, 2);
    ASSERT_EQ(stack->head->data, 1);
    ASSERT_EQ(stack->size, 2);
    pushToStack(stack, 5);
    ASSERT_EQ(stack->head->data, 1);
    ASSERT_EQ(stack->size, 3);

    ASSERT_EQ(popFromStack(stack), 5);
    ASSERT_EQ(stack->head->data, 1);
    ASSERT_EQ(stack->size, 2);
    ASSERT_EQ(popFromStack(stack), 2);
    ASSERT_EQ(stack->head->data, 1);
    ASSERT_EQ(stack->size, 1);
    ASSERT_EQ(popFromStack(stack), 1);
    ASSERT_EQ(stack->head, nullptr);
    ASSERT_EQ(stack->size, 0);
    ASSERT_EQ(popFromStack(stack), -999);
    free(stack);
}

TEST(ListTest, MakeListCopyTest) {
    Stack *stack = createStack();
    pushToStack(stack, 1);
    pushToStack(stack, 2);
    pushToStack(stack, 5);
    Stack *copy = makeStackCopy(stack);
    pushToStack(stack, 10);
    pushToStack(stack, 20);
    pushToStack(stack, 30);
    ASSERT_TRUE(popFromStack(copy) == 5);
    ASSERT_TRUE(popFromStack(copy) == 2);
    ASSERT_TRUE(popFromStack(copy) == 1);
    ASSERT_TRUE(popFromStack(copy) == -999);
    freeStack(copy);
    freeStack(stack);
}

TEST(ListTest, NullListToArrayTest) {
    Stack *stack = nullptr;
    size_t array[10];
    toArray(stack, array);
    freeStack(stack);
}

TEST(ListTest, PrintNullListTest) {
    Stack *stack = nullptr;
    printStack(stack);
    freeStack(stack);
}

TEST(ListTest, FreeNullListTest) {
    Stack *stack = nullptr;
    freeStack(stack);
}

// Graph tests ========================

TEST(GraphTest, CreateGraphTest) {
    Graph *graph = createGraph(1);
    ASSERT_NE(graph, nullptr);
    ASSERT_NE(graph->adjacencyList, nullptr);
    ASSERT_EQ(graph->size, 1);
    freeGraph(graph);
}

TEST(GraphTest, CheckGraphConnectivityTest) {
    Graph *connected = createGraph(5);

    addLine(connected, 0, 1);
    addLine(connected, 0, 4);
    addLine(connected, 0, 2);
    addLine(connected, 1, 2);
    addLine(connected, 1, 3);
    addLine(connected, 4, 1);
    ASSERT_TRUE(isGraphConnected(connected) == 1);

    Graph *anotherConnected = createGraph(4);
    addLine(anotherConnected, 0, 1);
    addLine(anotherConnected, 2, 1);
    addLine(anotherConnected, 3, 1);
    ASSERT_TRUE(isGraphConnected(anotherConnected) == 1);

    Graph *notConnected = createGraph(8);
    addLine(notConnected, 0, 1);
    addLine(notConnected, 1, 5);
    addLine(notConnected, 2, 5);
    addLine(notConnected, 4, 5);
    addLine(notConnected, 3, 5);
    addLine(notConnected, 6, 7);
    ASSERT_TRUE(isGraphConnected(notConnected) == 0);

    freeGraph(connected);
    freeGraph(anotherConnected);
    freeGraph(notConnected);
}

TEST(GraphTest, CheckGraphCyclesTest) {
    Graph *cyclic = createGraph(7);
    addLine(cyclic, 0, 1);
    addLine(cyclic, 1, 2);
    addLine(cyclic, 2, 3);
    addLine(cyclic, 3, 4);
    addLine(cyclic, 4, 3);
    addLine(cyclic, 3, 5);
    addLine(cyclic, 5, 1);
    addLine(cyclic, 1, 6);
    addLine(cyclic, 6, 0);

    Stack *longestCycle = getLongestGraphCycle(cyclic);
    size_t cycleInArray[10];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(0, 1, 2, 3, 4, 3, 5, 1, 6, 0));
    freeStack(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, LongestCycleNotFromFirstNodeTest) {
    Graph *cyclic = createGraph(8);
    addLine(cyclic, 1, 0);
    addLine(cyclic, 1, 2);
    addLine(cyclic, 2, 3);
    addLine(cyclic, 3, 4);
    addLine(cyclic, 4, 3);
    addLine(cyclic, 3, 5);
    addLine(cyclic, 5, 6);
    addLine(cyclic, 6, 1);
    addLine(cyclic, 0, 7);
    addLine(cyclic, 7, 0);
    Stack *longestCycle = getLongestGraphCycle(cyclic);
    size_t cycleInArray[8];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(1, 2, 3, 4, 3, 5, 6, 1));
    freeStack(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, GetLongestCycleOnSelfTest) {
    Graph *cyclic = createGraph(1);
    addLine(cyclic, 0, 0);
    Stack *longestCycle = getLongestGraphCycle(cyclic);
    size_t cycleInArray[2];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(0, 0));
    freeStack(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, NoCycleTest) {
    Graph *cyclic = createGraph(2);
    addLine(cyclic, 0, 1);
    Stack *longestCycle = getLongestGraphCycle(cyclic);
    ASSERT_TRUE(longestCycle == nullptr);
    freeGraph(cyclic);
}

TEST(GraphTest, LongCycleGraphTest) {
    Graph *cyclic = createGraph(17);
    addLine(cyclic, 0, 1);
    addLine(cyclic, 1, 2);
    addLine(cyclic, 2, 3);
    addLine(cyclic, 3, 4);
    addLine(cyclic, 4, 5);
    addLine(cyclic, 5, 6);
    addLine(cyclic, 5, 7);
    addLine(cyclic, 6, 7);
    addLine(cyclic, 7, 8);
    addLine(cyclic, 8, 9);
    addLine(cyclic, 9, 10);
    addLine(cyclic, 10, 11);
    addLine(cyclic, 11, 12);
    addLine(cyclic, 12, 13);
    addLine(cyclic, 13, 4);
    addLine(cyclic, 4, 14);
    addLine(cyclic, 14, 15);
    addLine(cyclic, 15, 16);
    addLine(cyclic, 16, 0);

    Stack *longestCycle = getLongestGraphCycle(cyclic);
    size_t cycleInArray[18];
    toArray(longestCycle, cycleInArray);
    size_t expectedCycle[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 4, 14, 15, 16, 0};
    ASSERT_TRUE(arraysCompare(cycleInArray, expectedCycle, 18));
    freeStack(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, AddLineToNonExistentNodeTest) {
    Graph *graph = createGraph(3);
    addLine(graph, 3, 5);
    addLine(graph, 10, 20);
    addLine(graph, 2, 3);
    ASSERT_TRUE(graph->adjacencyList[2]->head->next == nullptr);
    freeGraph(graph);
}

TEST(GraphTest, TwoEqualCyclesTest) {
    Graph *graph = createGraph(5);
    addLine(graph, 0, 1);
    addLine(graph, 1, 2);
    addLine(graph, 2, 0);

    addLine(graph, 0, 3);
    addLine(graph, 3, 4);
    addLine(graph, 4, 0);

    Stack *longestCycle = getLongestGraphCycle(graph);
    size_t cycleInArray[7];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(1, 2, 0, 3, 4, 0, 1));
    freeStack(longestCycle);
    freeGraph(graph);
}

TEST(GraphTest, DFSStartingFromNonExistentNodeTest) {
    Graph *graph = createGraph(3);
    size_t visitedNodes[graph->size];
    tryToVisitAllNodes(graph, 5, visitedNodes, 0);
    freeGraph(graph);
}

TEST(GraphTest, GetLongestCycleFromNonExistentNodeTest) {
    Graph *graph = createGraph(3);
    CyclesList *cycles = createCyclesList();
    for (size_t i = 0; i < graph->size + 5; i++) { // loop with overflow
        size_t visited[graph->size];
        memset(visited, 0, sizeof(visited));
        Stack *stack = createStack();
        getAllCyclesFromNode(graph, i, stack, visited, cycles);
        freeStack(stack);
    }
    freeCyclesList(cycles);
    freeGraph(graph);
}

TEST(GraphTest, FreeNullGraphTest) {
    Graph *graph = nullptr;
    freeGraph(graph);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/graph.c"
#include "test_utils.cpp"

// List tests =========================

TEST(ListTest, CreateListTest) {
    List *list = createList();
    ASSERT_NE(list, nullptr);
    ASSERT_EQ(list->head, nullptr);
    free(list);
}

TEST(ListTest, AddToList) {
    List *list = createList();
    ASSERT_EQ(list->head, nullptr);
    ASSERT_EQ(list->size, 0);
    addToList(list, 1);
    ASSERT_NE(list->head, nullptr);
    ASSERT_EQ(list->head->data, 1);
    ASSERT_EQ(list->size, 1);
    addToList(list, 2);
    ASSERT_EQ(list->head->data, 1);
    ASSERT_EQ(list->size, 2);
    addToList(list, 5);
    ASSERT_EQ(list->head->data, 1);
    ASSERT_EQ(list->size, 3);
    freeList(list);
}

TEST(ListTest, PopFromList) {
    // TODO: Replace manual filling with variadic constructor
    List *list = createList();
    ASSERT_EQ(list->head, nullptr);
    ASSERT_EQ(list->size, 0);
    addToList(list, 1);
    ASSERT_NE(list->head, nullptr);
    ASSERT_EQ(list->head->data, 1);
    ASSERT_EQ(list->size, 1);
    addToList(list, 2);
    ASSERT_EQ(list->head->data, 1);
    ASSERT_EQ(list->size, 2);
    addToList(list, 5);
    ASSERT_EQ(list->head->data, 1);
    ASSERT_EQ(list->size, 3);

    ASSERT_EQ(popFromList(list), 5);
    ASSERT_EQ(list->head->data, 1);
    ASSERT_EQ(list->size, 2);
    ASSERT_EQ(popFromList(list), 2);
    ASSERT_EQ(list->head->data, 1);
    ASSERT_EQ(list->size, 1);
    ASSERT_EQ(popFromList(list), 1);
    ASSERT_EQ(list->head, nullptr);
    ASSERT_EQ(list->size, 0);
    ASSERT_EQ(popFromList(list), -999);
    free(list);
}

TEST(ListTest, MakeListCopyTest) {
    List *list = createList();
    addToList(list, 1);
    addToList(list, 2);
    addToList(list, 5);
    List *copy = makeCopy(list);
    addToList(list, 10);
    addToList(list, 20);
    addToList(list, 30);
    ASSERT_TRUE(popFromList(copy) == 5);
    ASSERT_TRUE(popFromList(copy) == 2);
    ASSERT_TRUE(popFromList(copy) == 1);
    ASSERT_TRUE(popFromList(copy) == -999);
    freeList(copy);
    freeList(list);
}

TEST(ListTest, FreeNullListTest) {
    List *list = nullptr;
    freeList(list);
}

// Graph tests ========================

TEST(GraphTest, CreateGraphTest) {
    Graph *graph = createGraph(1);
    ASSERT_NE(graph, nullptr);
    ASSERT_NE(graph->adjacencyLists, nullptr);
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

    List *longestCycle = getLongestGraphCycle(cyclic);
    int cycleInArray[10];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(0, 1, 2, 3, 4, 3, 5, 1, 6, 0));
    freeList(longestCycle);
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
    List *longestCycle = getLongestGraphCycle(cyclic);
    int cycleInArray[8];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(1, 2, 3, 4, 3, 5, 6, 1));
    freeList(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, GetLongestCycleOnSelfTest) {
    Graph *cyclic = createGraph(1);
    addLine(cyclic, 0, 0);
    List *longestCycle = getLongestGraphCycle(cyclic);
    int cycleInArray[2];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(0, 0));
    freeList(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, NoCycleTest) {
    Graph *cyclic = createGraph(2);
    addLine(cyclic, 0, 1);
    List *longestCycle = getLongestGraphCycle(cyclic);
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

    List *longestCycle = getLongestGraphCycle(cyclic);
    int cycleInArray[18];
    toArray(longestCycle, cycleInArray);
    int expectedCycle[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 4, 14, 15, 16, 0};
    ASSERT_TRUE(arraysCompare(cycleInArray, expectedCycle, 18));
    freeList(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, FreeNullGraphTest) {
    Graph *graph = nullptr;
    freeGraph(graph);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/graph.c"

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

// Graph tests ========================

TEST(GraphTest, CreateGraphTest) {
    Graph *graph = createGraph(1);
    ASSERT_NE(graph, nullptr);
    ASSERT_NE(graph->adjacencyLists, nullptr);
    ASSERT_EQ(graph->size, 0);
    freeGraph(graph);
}

TEST(GraphTest, CheckGraphConnectivityTest) {
    Graph *connected = createGraph(5);
    addNode(connected, 1);
    addNode(connected, 5);
    addNode(connected, 10);
    addNode(connected, 11);
    addNode(connected, 6);
    addLine(connected, 1, 5);
    addLine(connected, 1, 6);
    addLine(connected, 1, 10);
    addLine(connected, 5, 10);
    addLine(connected, 5, 11);
    addLine(connected, 6, 5);
    ASSERT_TRUE(isGraphConnected(connected) == 1);

    Graph *anotherConnected = createGraph(4);
    addNode(anotherConnected, 1);
    addNode(anotherConnected, 5);
    addNode(anotherConnected, 6);
    addNode(anotherConnected, 11);
    addLine(anotherConnected, 1, 5);
    addLine(anotherConnected, 6, 5);
    addLine(anotherConnected, 11, 5);
    ASSERT_TRUE(isGraphConnected(anotherConnected) == 1);

    Graph *notConnected = createGraph(8);
    addNode(notConnected, 1);
    addNode(notConnected, 5);
    addNode(notConnected, 4);
    addNode(notConnected, 3);
    addNode(notConnected, 2);
    addNode(notConnected, 6);
    addNode(notConnected, 9);
    addNode(notConnected, 10);
    addLine(notConnected, 1, 5);
    addLine(notConnected, 2, 5);
    addLine(notConnected, 4, 5);
    addLine(notConnected, 3, 5);
    addLine(notConnected, 6, 3);
    addLine(notConnected, 9, 10);
    ASSERT_TRUE(isGraphConnected(notConnected) == 0);

    freeGraph(connected);
    freeGraph(anotherConnected);
    freeGraph(notConnected);
}

TEST(GraphTest, CheckGraphCyclesTest) {
    Graph *cyclic = createGraph(6);
    addNode(cyclic, 1);
    addNode(cyclic, 2);
    addNode(cyclic, 3);
    addNode(cyclic, 4);
    addNode(cyclic, 5);
    addNode(cyclic, 6);
    addLine(cyclic, 1, 2);
    addLine(cyclic, 2, 3);
    addLine(cyclic, 3, 4);
    addLine(cyclic, 4, 3);
    addLine(cyclic, 3, 5);
    addLine(cyclic, 5, 2);
    addLine(cyclic, 2, 6);
    addLine(cyclic, 6, 1);

    List *longestCycle = getLongestCycle(cyclic);
    int cycleInArray[9];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(1, 2, 3, 4, 3, 5, 2, 6, 1));
    freeList(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, GetLongestCycleOnSelfTest) {
    Graph *cyclic = createGraph(1);
    addNode(cyclic, 1);
    addLine(cyclic, 1, 1);
    List *longestCycle = getLongestCycle(cyclic);
    int cycleInArray[2];
    toArray(longestCycle, cycleInArray);
    ASSERT_THAT(cycleInArray, testing::ElementsAre(1, 1));
    freeList(longestCycle);
    freeGraph(cyclic);
}

TEST(GraphTest, NoCycleTest) {
    Graph *cyclic = createGraph(2);
    addNode(cyclic, 1);
    addNode(cyclic, 2);
    addLine(cyclic, 1, 2);
    List *longestCycle = getLongestCycle(cyclic);
    ASSERT_TRUE(longestCycle == nullptr);
    freeGraph(cyclic);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

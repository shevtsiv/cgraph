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
    free(list);
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
    free(graph);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

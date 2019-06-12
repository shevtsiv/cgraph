#include "list.c"

typedef struct {
    size_t size;
    List *adjacencyLists;
} Graph;

Graph *createGraph(size_t capacity) {
    Graph *graph = (Graph *) calloc(1, sizeof(Graph));
    graph->adjacencyLists = (List *) malloc(sizeof(List) * capacity);
    return graph;
}

ListNode *newNode(Graph *graph, int data) {
    ListNode *newListNode = (ListNode *) calloc(1, sizeof(ListNode));
    newListNode->data = data;
    graph->adjacencyLists[graph->size] = *createList();
    addToList(&graph->adjacencyLists[graph->size++], data);
    return newListNode;
}

int indexOf(List array[], size_t arraySize, int value) {
    for (size_t i = 0; i < arraySize; i++) {
        if (array[i].head->data == value) {
            return i;
        }
    }
    return -1;
}

void addLine(Graph *graph, int sourceValue, int destinationValue) {
    size_t sourceIndex = indexOf(graph->adjacencyLists, graph->size, sourceValue);
    if (sourceIndex == -1) {

    }
    addToList(&graph->adjacencyLists[sourceIndex], destinationValue);
}

void printGraph(Graph *graph) {
    for (size_t i = 0; i < graph->size; i++) {
        printf("List №%li:\n", i);
        printList(&graph->adjacencyLists[i]);
        printf("\n");
    }
}

#include "list.c"

typedef struct {
    size_t size;
    List **adjacencyLists;
} Graph;

Graph *createGraph(size_t capacity) {
    Graph *graph = (Graph *) calloc(1, sizeof(Graph));
    graph->adjacencyLists = (List **) malloc(sizeof(List *) * capacity);
    return graph;
}

void addNode(Graph *graph, int data) {
    graph->adjacencyLists[graph->size] = createList();
    addToList(graph->adjacencyLists[graph->size++], data);
}

void addLine(Graph *graph, int sourceValue, int destinationValue) {
    size_t sourceIndex = indexOf(graph->adjacencyLists, graph->size, sourceValue);
    if (sourceIndex == -1) {

    }
    addToList(graph->adjacencyLists[sourceIndex], destinationValue);
}

void printGraph(Graph *graph) {
    for (size_t i = 0; i < graph->size; i++) {
        printf("List №%li:\n", i);
        printList(graph->adjacencyLists[i]);
        printf("\n");
    }
}

void freeGraph(Graph *graph) {
    for (size_t i = 0; i < graph->size; i++) {
        freeList(graph->adjacencyLists[i]);
    }
    free(graph->adjacencyLists);
    free(graph);
}

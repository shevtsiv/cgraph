#include "list.c"

typedef struct {
    size_t size;
    size_t capacity;
    List **adjacencyLists;
} Graph;

Graph *createGraph(size_t capacity) {
    Graph *graph = (Graph *) calloc(1, sizeof(Graph));
    graph->capacity = capacity;
    graph->adjacencyLists = (List **) malloc(sizeof(List *) * capacity);
    return graph;
}

void addNode(Graph *graph, int data) {
    graph->adjacencyLists[graph->size] = createList();
    if ((graph->size + 1) == graph->capacity) {
        List **newAdjacencyList = (List **) malloc(sizeof(List *) * graph->capacity * 2);
        for (size_t i = 0; i < graph->capacity; i++) {
            newAdjacencyList[i] = graph->adjacencyLists[i];
        }
        free(graph->adjacencyLists);
        graph->adjacencyLists = newAdjacencyList;
        graph->capacity = graph->capacity * 2;
    }
    addToList(graph->adjacencyLists[graph->size++], data);
}

// TODO: Use indexes instead of values?
void addLine(Graph *graph, int sourceValue, int destinationValue) {
    size_t sourceIndex = indexOf(graph->adjacencyLists, graph->size, sourceValue);
    if (sourceIndex == -1 || indexOf(graph->adjacencyLists, graph->size, destinationValue) == -1) {
        return;
    }
    addToList(graph->adjacencyLists[sourceIndex], destinationValue);
}

size_t indexOfNode(Graph *graph, int nodeValue) {
    for (size_t i = 0; i < graph->size; i++) {
        if (graph->adjacencyLists[i]->head->data == nodeValue) {
            return i;
        }
    }
    return -1;
}

void DFS(Graph *graph, int *componentsAmount, int value, int visitedNodes[]) {
    size_t valueIndex = indexOfNode(graph, value);
    if (valueIndex == -1) {
        return;
    }
    visitedNodes[valueIndex] = 1;
    if (valueIndex != 0) {
        (*componentsAmount)--;
    }
    printf("Visited: %i\n", value);
    ListNode *node = graph->adjacencyLists[valueIndex]->head->next;
    while (node != NULL) {
        size_t valueIndex1 = indexOfNode(graph, node->data);
        if (visitedNodes[valueIndex1] == 0) {
            DFS(graph, componentsAmount, node->data, visitedNodes);
        }
        node = node->next;
    }
}

void DFSSecond(Graph *graph, int *amount, int *componentsAmount, int value, int visitedNodes[]) {
    size_t valueIndex = indexOfNode(graph, value);
    if (valueIndex == -1) {
        return;
    }
    (*amount)++;

    ListNode *node = graph->adjacencyLists[valueIndex]->head->next;
    if (node == NULL) {
        (*componentsAmount) += *amount - 1;
        return;
    }
    if (visitedNodes[valueIndex] == 1) {
        if (valueIndex != 0) {
            (*componentsAmount)--;
        }
        return;
    }

    visitedNodes[valueIndex] = 1;

    if (valueIndex != 0) {
        (*componentsAmount)--;
    }
    printf("Visited: %i\n", value);
    while (node != NULL) {
        size_t valueIndex1 = indexOfNode(graph, node->data);
        if (visitedNodes[valueIndex1] == 0) {
            DFSSecond(graph, amount, componentsAmount, node->data, visitedNodes);
        }
        node = node->next;
    }
}

int isGraphConnected(Graph *graph) {
    int visitedNodes[graph->size];
    for (size_t i = 0; i < graph->size; i++) {
        visitedNodes[i] = 0;
    }
    int componentsAmount = graph->size;

    DFS(graph, &componentsAmount, graph->adjacencyLists[0]->head->data, visitedNodes);
    if (componentsAmount == 1) {
        return 1;
    }
    int a = 0;
    for (size_t i = 1; i < graph->size; i++) {
        DFSSecond(graph, &a, &componentsAmount, graph->adjacencyLists[i]->head->data, visitedNodes);
        a = 0;
    }
    if (componentsAmount == 1) {
        return 1;
    }
    return 0;
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

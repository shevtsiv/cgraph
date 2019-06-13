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

void DFS(Graph *graph, int valueIndex, int visitedNodes[], int mode) {
    ListNode *nextNode = graph->adjacencyLists[valueIndex]->head->next;
    if (mode == 1 && nextNode == NULL) {
        return;
    }
    visitedNodes[valueIndex] = 1;
    while (nextNode != NULL) {
        // TODO: If we can rely on valueIndex + 1 to get next node, we don't have to do indexOf here
        size_t nextNodeIndex = indexOfNode(graph, nextNode->data);
        if (visitedNodes[nextNodeIndex] == 0) {
            DFS(graph, nextNodeIndex, visitedNodes, mode);
        }
        nextNode = nextNode->next;
    }
}

int checkAllVisited(const int visitedNodes[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (visitedNodes[i] == 0) {
            return 0;
        }
    }
    return 1;
}

int isGraphConnected(Graph *graph) {
    int visitedNodes[graph->size];
    for (size_t i = 0; i < graph->size; i++) {
        visitedNodes[i] = 0;
    }
    DFS(graph, 0, visitedNodes, 0);
    if (checkAllVisited(visitedNodes, graph->size) == 1) {
        return 1;
    }
    for (size_t i = 1; i < graph->size; i++) {
        DFS(graph, i, visitedNodes, 1);
    }
    if (checkAllVisited(visitedNodes, graph->size) == 1) {
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

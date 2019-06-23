#include "list.c"
#include "utils.c"

typedef struct {
    size_t size;
    List **adjacencyLists;
} Graph;

Graph *createGraph(size_t nodesAmount) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->size = nodesAmount;
    graph->adjacencyLists = (List **) malloc(sizeof(List *) * nodesAmount);
    for (size_t i = 0; i < nodesAmount; i++) {
        graph->adjacencyLists[i] = createList();
        addToList(graph->adjacencyLists[i], i);
    }
    return graph;
}

void addLine(Graph *graph, int sourceIndex, int destinationIndex) {
    graph->adjacencyLists[sourceIndex]->head->edgesCount++;
    addToList(graph->adjacencyLists[sourceIndex], destinationIndex);
}

void tryToVisitAllNodes(Graph *graph, int startIndex, int *visitedNodes, size_t iteration) {
    if (visitedNodes[startIndex] == 1) {
        return;
    }
    ListNode *nextNode = graph->adjacencyLists[startIndex]->head->next;
    if (nextNode == NULL && iteration != 0) { // It is OK if we cannot access some node on the first iteration
        return;
    }
    visitedNodes[startIndex] = 1;
    while (nextNode != NULL) {
        tryToVisitAllNodes(graph, nextNode->data, visitedNodes, iteration);
        nextNode = nextNode->next;
    }
}

void getAllCycles(Graph *graph, int currentIndex, int visited[], int startNodeData, List *cycles[], size_t *cyclesAmount) {
    if (currentIndex == graph->size) {
        return;
    }
    ListNode *node = graph->adjacencyLists[currentIndex]->head;
    if (visited[currentIndex]) {
        if (node->data == startNodeData) {
            addToList(cycles[(*cyclesAmount)++], node->data);
            cycles[*cyclesAmount] = createList();
            return;
        }
        if (node->edgesCount == 1) {
            return;
        }
    }
    visited[currentIndex] = 1;
    addToList(cycles[*cyclesAmount], node->data);
    ListNode *nextNode = node->next;
    while (nextNode != NULL) {
        getAllCycles(graph, nextNode->data, visited, startNodeData, cycles, cyclesAmount);
        nextNode = nextNode->next;
    }
    popFromList(cycles[*cyclesAmount]);
}

List* getLongestCycle(Graph *graph) {
    List *cycles[100];
    cycles[0] = createList();
    size_t cyclesAmount = 0;
    for (size_t i = 0; i < graph->size; i++) {
        int visited[graph->size];
        for (size_t j = 0; j < graph->size; j++) {
            visited[j] = 0;
        }
        getAllCycles(graph, i, visited, graph->adjacencyLists[i]->head->data, cycles, &cyclesAmount);
    }
    if (cyclesAmount == 0) {
        freeList(cycles[0]);
        return NULL;
    }
    size_t maxCycleIndex = 0;
    for (size_t i = 1; i <= cyclesAmount; i++) {
        if (cycles[i]->size > cycles[maxCycleIndex]->size) {
            maxCycleIndex = i;
        } else {
            freeList(cycles[i]);
        }
    }
    return cycles[maxCycleIndex];
}

int isGraphConnected(Graph *graph) {
    int visitedNodes[graph->size];
    for (size_t i = 0; i < graph->size; i++) {
        visitedNodes[i] = 0;
    }
    for (size_t i = 0; i < graph->size; i++) {
        tryToVisitAllNodes(graph, i, visitedNodes, i);
    }
    return checkAllVisited(visitedNodes, graph->size);
}

void printGraph(Graph *graph) {
    for (size_t i = 0; i < graph->size; i++) {
        printf("List №%lu:\n", i);
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

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

void addLine(Graph *graph, size_t sourceIndex, size_t destinationIndex) {
    graph->adjacencyLists[sourceIndex]->head->edgesCount++;
    addToList(graph->adjacencyLists[sourceIndex], destinationIndex);
}

void tryToVisitAllNodes(Graph *graph, size_t startIndex, size_t *visitedNodes, size_t iteration) {
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

void getLongestCycle(Graph *graph, size_t currentIndex, size_t *visited, size_t startNodeData, List **longestCycle,
                     size_t *cyclesAmount) {
    if (currentIndex == graph->size) {
        return;
    }
    ListNode *node = graph->adjacencyLists[currentIndex]->head;
    if (visited[currentIndex]) {
        if (node->data == startNodeData) {
            addToList(longestCycle[*cyclesAmount], node->data);
            if (*cyclesAmount == 1) {
                if (longestCycle[1]->size > longestCycle[0]->size) {
                    freeList(longestCycle[0]);
                    longestCycle[0] = longestCycle[1];
                    longestCycle[1] = createList();
                } else {
                    freeList(longestCycle[1]);
                    longestCycle[1] = createList();
                }
                return;
            }
            ++(*cyclesAmount);
            return;
        }
        if (node->edgesCount == 1) {
            return;
        }
    }
    visited[currentIndex] = 1;
    addToList(longestCycle[*cyclesAmount], node->data);
    ListNode *nextNode = node->next;
    while (nextNode != NULL) {
        getLongestCycle(graph, nextNode->data, visited, startNodeData, longestCycle, cyclesAmount);
        nextNode = nextNode->next;
    }
    popFromList(longestCycle[*cyclesAmount]);
}

List *getLongestGraphCycle(Graph *graph) {
    List *cycles[2] = {createList(), createList()};
    size_t cyclesAmount = 0;
    for (size_t i = 0; i < graph->size; i++) {
        size_t visited[graph->size];
        memset(visited, 0, sizeof(visited));
        getLongestCycle(graph, i, visited, i, cycles, &cyclesAmount);
    }
    freeList(cycles[1]);
    if (cyclesAmount == 0) {
        freeList(cycles[0]);
        return NULL;
    }
    return cycles[0];
}

size_t isGraphConnected(Graph *graph) {
    size_t visitedNodes[graph->size];
    memset(visitedNodes, 0, sizeof(visitedNodes));
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
    if (graph == NULL) {
        return;
    }
    for (size_t i = 0; i < graph->size; i++) {
        freeList(graph->adjacencyLists[i]);
    }
    free(graph->adjacencyLists);
    free(graph);
}

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

// TODO: FIXME
// This func actually doesn't return all cycles since we have to rollback more nodes after reaching the end.
int getAllCycles(Graph *graph, int startIndex, List visitedNodes[], int visited[], int startNodeData, List *cycles[], size_t *cyclesAmount) {
    ListNode *node = graph->adjacencyLists[startIndex]->head;
    if (visited[startIndex] == 1) {
        if (node->data == startNodeData) {
            addToList(visitedNodes, node->data);
            printList(visitedNodes);
            cycles[*cyclesAmount] = createList();
            ListNode *cycleNode = visitedNodes->head;
            while (cycleNode != NULL) {
                addToList(cycles[*cyclesAmount], cycleNode->data);
                cycleNode = cycleNode->next;
            }
            (*cyclesAmount)++;
            // TODO: Remove this print, it is for debug purpose only
            printf("\n");
            return 1;
        }
        if (node->edgesCount == 1) {
            return -1;
        }
    }
    visited[startIndex] = 1;
    addToList(visitedNodes, node->data);
    ListNode *nextNode = node->next;
    if (nextNode == NULL || ((startIndex + 1) == graph->size && nextNode->data != startNodeData)) {
        popFromList(visitedNodes);
        return -1;
    }
    while (nextNode != NULL) {
        int i = getAllCycles(graph, nextNode->data, visitedNodes, visited, startNodeData, cycles, cyclesAmount);
        if (i == 1) {
            popFromList(visitedNodes);
        }
        nextNode = nextNode->next;
    }
    popFromList(visitedNodes);
    return -1;
}

List* getLongestCycle(Graph *graph) {
    List *cycles[100];
    size_t cyclesAmount = 0;
    for (size_t i = 0; i < graph->size; i++) {
        int visited[graph->size];
        for (size_t j = 0; j < graph->size; j++) {
            visited[j] = 0;
        }
        List *list = createList();

        getAllCycles(graph, i, list, visited, graph->adjacencyLists[i]->head->data, cycles, &cyclesAmount);
        freeList(list);
    }
    if (cyclesAmount == 0) {
        return NULL;
    }
    size_t maxCycleIndex = 0;
    for (size_t i = 1; i < cyclesAmount; i++) {
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

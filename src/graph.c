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

size_t indexOfNode(Graph *graph, int nodeValue) {
    for (size_t i = 0; i < graph->size; i++) {
        if (graph->adjacencyLists[i]->head->data == nodeValue) {
            return i;
        }
    }
    return -1;
}

void addLine(Graph *graph, int sourceValue, int destinationValue) {
    size_t sourceIndex = indexOf(graph->adjacencyLists, graph->size, sourceValue);
    graph->adjacencyLists[sourceIndex]->head->edgesCount++;
    if (sourceIndex == -1 || indexOf(graph->adjacencyLists, graph->size, destinationValue) == -1) {
        return;
    }
    addToList(graph->adjacencyLists[sourceIndex], destinationValue);
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

// TODO: FIXME
// This func actually doesn't return all cycles since we have to rollback more nodes after reaching the end.
int getAllCycles(Graph *graph, int valueIndex, List visitedNodes[], int visited[], int startNodeData, List *cycles[], size_t *cyclesAmount) {
    ListNode *node = graph->adjacencyLists[valueIndex]->head;
    if (visited[indexOfNode(graph, node->data)] == 1) {
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
    visited[indexOfNode(graph, node->data)] = 1;
    addToList(visitedNodes, node->data);
    ListNode *nextNode = node->next;
    while (nextNode != NULL) {
        size_t nextNodeIndex = indexOfNode(graph, nextNode->data);
        int i = getAllCycles(graph, nextNodeIndex, visitedNodes, visited, startNodeData, cycles, cyclesAmount);
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

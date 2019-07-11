#include "list.c"
#include "cycles_list.c"
#include "utils.c"

typedef struct {
    size_t size;
    List **adjacencyLists;
} Graph;

Graph *createGraph(size_t nodesAmount) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->size = nodesAmount;
    graph->adjacencyLists = (List **) malloc(sizeof(List *) * nodesAmount);
    if (graph->adjacencyLists == NULL) {
        free(graph);
        return NULL;
    }
    for (size_t i = 0; i < nodesAmount; i++) {
        List *list = createList();
        if (list == NULL) {
            free(graph->adjacencyLists);
            free(graph);
            return NULL;
        }
        graph->adjacencyLists[i] = list;
        addToList(graph->adjacencyLists[i], i);
    }
    return graph;
}

void addLine(Graph *graph, size_t sourceIndex, size_t destinationIndex) {
    if (graph == NULL || sourceIndex >= graph->size || destinationIndex >= graph->size) {
        return;
    }
    graph->adjacencyLists[sourceIndex]->head->edgesCount++;
    addToList(graph->adjacencyLists[sourceIndex], destinationIndex);
}

void tryToVisitAllNodes(Graph *graph, size_t startIndex, size_t *visitedNodes, size_t iteration) {
    if (graph == NULL || startIndex >= graph->size || visitedNodes[startIndex] == 1) {
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

int getLongestCycleFromNode(Graph *graph, size_t currentIndex, List *visitedNodes, size_t *visited,
                            size_t startNodeData, CyclesList *cycles) {
    if (graph == NULL || currentIndex >= graph->size) {
        return 0;
    }
    ListNode *node = graph->adjacencyLists[currentIndex]->head;
    if (visited[currentIndex]) {
        if (node->data == startNodeData) {
            addToList(visitedNodes, node->data);
            printList(visitedNodes);
            printf("\n");
            addToCyclesList(cycles, makeCopy(visitedNodes));
            return 1;
        }
        if (node->edgesCount == 1) {
            return 0;
        }
    }
    visited[currentIndex] = 1;
    addToList(visitedNodes, node->data);
    ListNode *nextNode = node->next;
    while (nextNode != NULL) {
        int i = getLongestCycleFromNode(graph, nextNode->data, visitedNodes, visited, startNodeData, cycles);
        if (i == 1) {
            popFromList(visitedNodes);
            visited[currentIndex] = 0;
        }
        nextNode = nextNode->next;
    }
    popFromList(visitedNodes);
    visited[currentIndex] = 0;
}

List *getLongestGraphCycle(Graph *graph) {
    if (graph == NULL) {
        return NULL;
    }
    CyclesList *cyclesList = createCyclesList();
    for (size_t i = 0; i < graph->size; i++) {
        size_t visited[graph->size];
        memset(visited, 0, sizeof(visited));
        List *list = createList();
        getLongestCycleFromNode(graph, i, list, visited, i, cyclesList);
        freeList(list);
    }
    if (cyclesList->size == 0) {
        freeCyclesList(cyclesList);
        return NULL;
    }
    List *maxLengthCycle = makeCopy(cyclesList->head->data);
    CyclesListNode *temp = cyclesList->head->next;
    while (temp != NULL) {
        if (temp->data->size > maxLengthCycle->size) {
            freeList(maxLengthCycle);
            maxLengthCycle = makeCopy(temp->data);
        }
        temp = temp->next;
    }
    freeCyclesList(cyclesList);
    return maxLengthCycle;
}

size_t isGraphConnected(Graph *graph) {
    if (graph == NULL) {
        return 0;
    }
    size_t visitedNodes[graph->size];
    memset(visitedNodes, 0, sizeof(visitedNodes));
    for (size_t i = 0; i < graph->size; i++) {
        tryToVisitAllNodes(graph, i, visitedNodes, i);
    }
    return checkAllVisited(visitedNodes, graph->size);
}

void printGraph(Graph *graph) {
    if (graph == NULL) {
        return;
    }
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

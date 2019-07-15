/**
 * This file contains main logic of this project: Graph data structure itself
 * and functions for working with this graph.
 * This graph is a directed, unweighted. Also, it can be either connected or not.
 * The main algorithm represented here is finding the longest cycle in this graph.
 */

#include "stack.c"
#include "cycles_list.c"
#include "utils.c"

/**
 * This structure represents graph itself. It consists of 2 fields:
 * size - Amount of vertices in a graph.
 * adjacencyList - Adjacency lists that represents the graph.
 */
typedef struct {
    size_t size;
    Stack **adjacencyList;
} Graph;

/**
 * This function is responsible for creating a graph with given nodes amount.
 * @param nodesAmount - Amount of nodes in a graph.
 * @return Graph* - if all the allocations have been done successfully.
 * @return NULL - if there were problems with allocations for Graph structure or adjacency stack for it.
 */
Graph *createGraph(size_t nodesAmount) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->size = nodesAmount;
    graph->adjacencyList = (Stack **) malloc(sizeof(Stack *) * nodesAmount);
    if (graph->adjacencyList == NULL) {
        free(graph);
        return NULL;
    }
    for (size_t i = 0; i < nodesAmount; i++) {
        Stack *stack = createStack();
        if (stack == NULL) {
            free(graph->adjacencyList);
            free(graph);
            return NULL;
        }
        graph->adjacencyList[i] = stack;
        pushToStack(graph->adjacencyList[i], i);
    }
    return graph;
}

/**
 * This function is responsible for creating a line between 2 nodes in a graph.
 * If graph is null or at least one of given indexes do not exist function does nothing.
 * @param graph - Graph where nodes are located.
 * @param sourceIndex - Number of node from which the line will be added.
 * @param destinationIndex - Number of node to which the line will be added.
 */
void addLine(Graph *graph, size_t sourceIndex, size_t destinationIndex) {
    if (graph == NULL || sourceIndex >= graph->size || destinationIndex >= graph->size) {
        return;
    }
    graph->adjacencyList[sourceIndex]->head->edgesCount++;
    pushToStack(graph->adjacencyList[sourceIndex], destinationIndex);
}

/**
 * This function is responsible for visiting maximum amount of nodes in a graph.
 * It works in 2 modes:
 * 1) If we cannot access all the nodes in a graph, it is OK, because the graph is directed.
 * In this mode all accessed nodes are marked as visited.
 * 2) In this mode we must reach visited node, otherwise there are multiple connectivity components,
 * graph is treated as not connected and the last one node is not marked as
 * visited - that is how calling function determines not connected graph.
 *
 * @param graph - Graph where start node is located.
 * @param startIndex - Number of start node.
 * @param visitedNodes - Array of size_t values, which uses 0 for not visited nodes and 1 for visited.
 * @param iteration - Number of iteration. If it is 0 function runs in the first mode, otherwise it runs in the second mode.
 */
void tryToVisitAllNodes(Graph *graph, size_t startIndex, size_t *visitedNodes, size_t iteration) {
    if (graph == NULL || startIndex >= graph->size || visitedNodes[startIndex] == 1) {
        return;
    }
    StackNode *nextNode = graph->adjacencyList[startIndex]->head->next;
    if (nextNode == NULL && iteration != 0) { // It is OK if we cannot access some node on the first iteration
        return;
    }
    visitedNodes[startIndex] = 1;
    while (nextNode != NULL) {
        tryToVisitAllNodes(graph, nextNode->data, visitedNodes, iteration);
        nextNode = nextNode->next;
    }
}

/**
 * This function is responsible for getting all cycles that are available from current node.
 * @param graph - Graph in which current node is located.
 * @param currentIndex - Number of node that starts a cycle.
 * @param visitedNodes - Array of size_t values, which uses 0 for not visited nodes and 1 for visited.
 * @param visited - Stack of visited nodes. If function finds the cycle this stack will be copied into cycles stack.
 * @param cycles - Stack of stacks to store all found cycles.
 * @return 1 if cycle was found, otherwise 0 is returned.
 */
int getAllCyclesFromNode(Graph *graph, size_t currentIndex, Stack *visitedNodes, size_t *visited, CyclesList *cycles) {
    if (graph == NULL || currentIndex >= graph->size) {
        return 0;
    }
    StackNode *node = graph->adjacencyList[currentIndex]->head;
    // If number of current node is bigger than initial node number and initial node has exactly one edge
    // then we have already found this cycle in previous iteration and there is no need to continue.
    if (visitedNodes->head != NULL && node->edgesCount == 1 && visitedNodes->head->data > node->data) {
        // TODO: This approach does not take into account cycles starting from node with multiple 'edgesCount'
        return 0;
    }
    if (visited[currentIndex]) {
        if (node->data == visitedNodes->head->data) {
            pushToStack(visitedNodes, node->data);
            printStack(visitedNodes);
            printf("\n");
            addToCyclesList(cycles, makeStackCopy(visitedNodes));
            return 1;
        }
        if (node->edgesCount == 1) {
            return 0;
        }
    }
    visited[currentIndex] = 1;
    pushToStack(visitedNodes, node->data);
    StackNode *nextNode = node->next;
    while (nextNode != NULL) {
        int i = getAllCyclesFromNode(graph, nextNode->data, visitedNodes, visited, cycles);
        if (i == 1) {
            popFromStack(visitedNodes);
            visited[currentIndex] = 0;
        }
        nextNode = nextNode->next;
    }
    popFromStack(visitedNodes);
    visited[currentIndex] = 0;
    return 0;
}


/**
 * This function is responsible for finding longest cycle in an entire graph.
 * @param graph - Graph where longest cycle is located.
 * @return - Stack with all nodes that form longest cycle or NULL if there are no cycles.
 */
Stack *getLongestGraphCycle(Graph *graph) {
    if (graph == NULL) {
        return NULL;
    }
    CyclesList *cyclesList = createCyclesList();
    for (size_t i = 0; i < graph->size; i++) {
        size_t visited[graph->size];
        memset(visited, 0, sizeof(visited));
        Stack *stack = createStack();
        getAllCyclesFromNode(graph, i, stack, visited, cyclesList);
        freeStack(stack);
    }
    if (cyclesList->size == 0) {
        freeCyclesList(cyclesList);
        return NULL;
    }
    Stack *maxLengthCycle = makeStackCopy(cyclesList->head->data);
    CyclesListNode *temp = cyclesList->head->next;
    while (temp != NULL) {
        if (temp->data->size > maxLengthCycle->size) {
            freeStack(maxLengthCycle);
            maxLengthCycle = makeStackCopy(temp->data);
        }
        temp = temp->next;
    }
    freeCyclesList(cyclesList);
    return maxLengthCycle;
}

/**
 * This function is responsible for graph connectivity determination.
 * @param graph - Graph that will be checked for connectivity.
 * @return 1 if graph is connected or 0 if it is not.
 */
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

/**
 * This function is responsible for entire graph deletion.
 * @param graph - Graph that will be deleted.
 */
void freeGraph(Graph *graph) {
    if (graph == NULL) {
        return;
    }
    for (size_t i = 0; i < graph->size; i++) {
        freeStack(graph->adjacencyList[i]);
    }
    free(graph->adjacencyList);
    free(graph);
}

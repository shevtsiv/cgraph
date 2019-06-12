#include <stdlib.h>
#include <stdio.h>
#include "list.c"
#include "graph.c"


int main() {
    Graph *graph = createGraph();
    ListNode *node1 = newNode(graph, 1);
    ListNode *node5 = newNode(graph, 5);
    ListNode *node10 = newNode(graph, 10);
    ListNode *node11 = newNode(graph, 11);
    ListNode *node6 = newNode(graph, 6);
    addLine(graph, 1, 5);
    addLine(graph, 1, 6);
    addLine(graph, 1, 10);

    addLine(graph, 5, 10);
    addLine(graph, 5, 11);

    addLine(graph, 6, 5);
    printGraph(graph);
    return 0;
}

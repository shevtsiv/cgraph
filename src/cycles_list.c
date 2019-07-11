typedef struct CyclesListNode {
    List *data;
    struct CyclesListNode *next;
} CyclesListNode;

typedef struct {
    CyclesListNode *head;
    size_t size;
} CyclesList;

CyclesList *createCyclesList() {
    CyclesList *cyclesList = (CyclesList *) calloc(1, sizeof(CyclesList));
    return cyclesList;
}

void addToCyclesList(CyclesList *cyclesList, List *list) {
    if (cyclesList == NULL) {
        return;
    }
    CyclesListNode *node = (CyclesListNode *) calloc(1, sizeof(CyclesListNode));
    if (node == NULL) {
        return;
    }
    node->data = list;
    if (cyclesList->head == NULL) {
        cyclesList->head = node;
        cyclesList->size++;
        return;
    }
    CyclesListNode *tail = cyclesList->head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = node;
    cyclesList->size++;
}

void freeCyclesList(CyclesList *list) {
    if (list == NULL) {
        return;
    }
    CyclesListNode *node = list->head;
    while (node != NULL) {
        CyclesListNode *next = node->next;
        freeList(node->data);
        free(node);
        node = next;
    }
    free(list);
}

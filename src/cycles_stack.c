typedef struct CyclesStackNode {
    Stack *data;
    struct CyclesStackNode *next;
} CyclesStackNode;

typedef struct {
    CyclesStackNode *head;
    size_t size;
} CyclesStack;

CyclesStack *createCyclesList() {
    CyclesStack *cyclesStack = (CyclesStack *) calloc(1, sizeof(CyclesStack));
    return cyclesStack;
}

void pushToCyclesStack(CyclesStack *cyclesStack, Stack *stack) {
    if (cyclesStack == NULL) {
        return;
    }
    CyclesStackNode *node = (CyclesStackNode *) calloc(1, sizeof(CyclesStackNode));
    if (node == NULL) {
        return;
    }
    node->data = stack;
    node->next = cyclesStack->head;
    cyclesStack->head = node;
    cyclesStack->size++;
}

void freeCyclesList(CyclesStack *stack) {
    if (stack == NULL) {
        return;
    }
    CyclesStackNode *node = stack->head;
    while (node != NULL) {
        CyclesStackNode *next = node->next;
        freeStack(node->data);
        free(node);
        node = next;
    }
    free(stack);
}

typedef struct StackNode {
    size_t data;
    struct StackNode *next;
    struct StackNode *prev;
    size_t edgesCount;
} StackNode;

typedef struct {
    StackNode *head;
    StackNode *tail;
    size_t size;
} Stack;

Stack *createStack() {
    Stack *stack = (Stack *) calloc(1, sizeof(Stack));
    return stack;
}

void pushToStack(Stack *stack, size_t data) {
    if (stack == NULL) {
        return;
    }
    StackNode *node = (StackNode *) calloc(1, sizeof(StackNode));
    if (node == NULL) {
        return;
    }
    node->data = data;
    if (stack->head == NULL) {
        stack->head = node;
        stack->tail = stack->head;
        stack->size++;
        return;
    }
    stack->tail->next = node;
    node->prev = stack->tail;
    stack->tail = node;
    stack->size++;
}

size_t popFromStack(Stack *stack) {
    if (stack == NULL || stack->head == NULL) {
        return -999;
    }
    StackNode *newTail = stack->tail->prev;
    size_t data = stack->tail->data;
    free(stack->tail);
    stack->size--;
    if (newTail == NULL) {
        stack->head = NULL;
        stack->tail = NULL;
    } else {
        newTail->next = NULL;
        stack->tail = newTail;
    }
    return data;
}

Stack *makeStackCopy(Stack *first) {
    if (first == NULL) {
        return NULL;
    }
    Stack *stack = createStack();
    if (stack == NULL) {
        return NULL;
    }
    StackNode *node = first->head;
    while (node != NULL) {
        pushToStack(stack, node->data);
        node = node->next;
    }
    return stack;
}

void toArray(Stack *stack, size_t array[]) {
    if (stack == NULL) {
        return;
    }
    StackNode *node = stack->head;
    size_t index = 0;
    while (node != NULL) {
        array[index++] = node->data;
        node = node->next;
    }
}

void printStack(Stack *stack) {
    if (stack == NULL) {
        return;
    }
    StackNode *node = stack->head;
    while (node != NULL) {
        printf("%lu ", node->data);
        node = node->next;
    }
}

void freeStack(Stack *stack) {
    if (stack == NULL) {
        return;
    }
    StackNode *node = stack->head;
    while (node != NULL) {
        StackNode *next = node->next;
        free(node);
        node = next;
    }
    free(stack);
}

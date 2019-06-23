typedef struct ListNode {
    size_t data;
    struct ListNode *next;
    size_t edgesCount;
} ListNode;

typedef struct {
    ListNode *head;
    size_t size;
} List;

List *createList() {
    List *list = (List *) calloc(1, sizeof(List));
    return list;
}

void addToList(List *list, size_t data) {
    if (list == NULL) {
        return;
    }
    ListNode *node = (ListNode *) calloc(1, sizeof(ListNode));
    if (node == NULL) {
        return;
    }
    node->data = data;
    if (list->head == NULL) {
        list->head = node;
        list->size++;
        return;
    }
    ListNode *tail = list->head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = node;
    list->size++;
}

size_t popFromList(List *list) {
    if (list == NULL || list->head == NULL) {
        return -999;
    }
    if (list->head->next == NULL) {
        size_t data = list->head->data;
        free(list->head);
        list->head = NULL;
        list->size = 0;
        return data;
    }
    ListNode *preTail = list->head;
    while (preTail->next->next != NULL) {
        preTail = preTail->next;
    }
    size_t data = preTail->next->data;
    free(preTail->next);
    preTail->next = NULL;
    list->size--;
    return data;
}

List *makeCopy(List *first) {
    if (first == NULL) {
        return NULL;
    }
    List *list = createList();
    if (list == NULL) {
        return NULL;
    }
    ListNode *node = first->head;
    while (node != NULL) {
        addToList(list, node->data);
        node = node->next;
    }
    return list;
}

void toArray(List *list, size_t array[]) {
    if (list == NULL) {
        return;
    }
    ListNode *node = list->head;
    size_t index = 0;
    while (node != NULL) {
        array[index++] = node->data;
        node = node->next;
    }
}

void printList(List *list) {
    if (list == NULL) {
        return;
    }
    ListNode *node = list->head;
    while (node != NULL) {
        printf("%lu ", node->data);
        node = node->next;
    }
}

void freeList(List *list) {
    if (list == NULL) {
        return;
    }
    ListNode *node = list->head;
    while (node != NULL) {
        ListNode *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

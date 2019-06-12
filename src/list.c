typedef struct ListNode {
    int data;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    size_t size;
} List;

List *createList() {
    List *list = (List *) calloc(1, sizeof(List));
    return list;
}

void addToList(List *list, int data) {
    ListNode *node = (ListNode *) calloc(1, sizeof(ListNode));
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

int popFromList(List *list) {
    if (list->head == NULL) {
        return -999;
    }
    if (list->head->next == NULL) {
        int data = list->head->data;
        free(list->head);
        list->head = NULL;
        list->size = 0;
        return data;
    }
    ListNode *preTail = list->head;
    while (preTail->next->next != NULL) {
        preTail = preTail->next;
    }
    int data = preTail->next->data;
    free(preTail->next);
    preTail->next = NULL;
    list->size--;
    return data;
}

void printList(List *list) {
    ListNode *node = list->head;
    while (node != NULL) {
        printf("%i ", node->data);
        node = node->next;
    }
}

void freeList(List *list) {
    ListNode *node = list->head;
    while (node != NULL) {
        ListNode *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

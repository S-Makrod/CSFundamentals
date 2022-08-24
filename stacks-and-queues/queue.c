#include <stdio.h>
#include <stdlib.h>

typedef struct queueNode {
    int val;
    struct queueNode *next;
    struct queueNode *prev;
} node;

typedef struct QueueHead {
    node *head;
    node *tail;
}queue;

queue *newQueue() {
    // allocate memory
    queue *head = (queue *)calloc(1, sizeof(queue));
    head->head = NULL;
    head->tail = NULL;

    return head;
}

node *newNode(int val) {
    // allocate memory
    node *new_node = (node *)calloc(1, sizeof(node));
    new_node->val = val;

    return new_node;
}

// prints stack
void printQueue(queue *head) {
    node *curr = head->head;

    while(curr != NULL){
        printf("|%d|\n", curr->val);
        printf("| ^\n");
        printf("v |\n");
        curr = curr->next;
    }

    printf("NULL\n");
}

queue *enqueue(queue *head, node *new) {
    if(head->head == NULL) {
        head->head = new;
        head->tail = new;
        return head;
    }

    new->prev = head->tail;
    head->tail->next = new;
    head->tail = new;

    return head;
}

queue *dequeue(queue *head, int *dequeued) {
    if(head->head == NULL) return NULL;

    *dequeued = head->head->val;

    node *temp = head->head;
    head->head = head->head->next;
    head->head->prev = NULL;
    free(temp);
    return head;
}

queue *freeQueue(queue *head) {
    node *curr = head->head;
    node *temp;

    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }

    free(head);
}

int main() {
    queue *head = newQueue();
    head = enqueue(head, newNode(5));
    head = enqueue(head, newNode(12));
    head = enqueue(head, newNode(56));
    head = enqueue(head, newNode(2));
    head = enqueue(head, newNode(9));
    head = enqueue(head, newNode(90));
    head = enqueue(head, newNode(-12));

    printQueue(head);

    int dequeued;

    printf("\n");

    head = dequeue(head, &dequeued);
    printf("The value %d was dequeued\n", dequeued);

    head = dequeue(head, &dequeued);
    printf("The value %d was dequeued\n", dequeued);

    printf("\n");
    printQueue(head);
    freeQueue(head);

    return 0;
}
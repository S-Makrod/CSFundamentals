#include <stdio.h>
#include <stdlib.h>

typedef struct linkedListNode {
    int val;
    struct linkedListNode * next;
} node;

// creates new node
node *newNode(int val) {
    // allocate memory
    node *new_node = (node *)calloc(1, sizeof(node));
    new_node->val = val;

    return new_node;
}

// prints list
void printList(node *head) {
    node *curr = head;

    while(curr != NULL){
        printf("|%d|->", curr->val);
        curr = curr->next;
    }

    printf("NULL\n");
}

// search for val, returns 1 if found 0 if not found
int search(node *head, int val) {
    node *curr = head;

    while(curr != NULL){
        if (curr->val == val) return 1; // found
        curr = curr->next;
    }

    // not fount
    return 0;
}

// returns length of list
int getLength(node *head) {
    int len = 0;
    node *curr = head;

    while(curr != NULL){
        len++;
        curr = curr->next;
    }

    return len;
}

// inserts at head of list, returns head
node *insertHead(node *head, node *new) {
    new->next = head;
    return new;
}

// inserts at tail of list, returns head
node *insertTail(node *head, node *new) {
    if(head == NULL) return new;

    node *curr = head;
    while(curr->next != NULL) curr = curr->next;
    curr->next = new;

    return head;
}

// insert at middle of list, specifically at index=pos, returns head
node *insertMiddle(node *head, node *new, int pos) {
    if(pos < 0) return head;
    if(pos == 0) return insertHead(head, new);

    node *curr = head;

    // find node at index=pos-1
    for(int index = 0; index < pos-1 && curr != NULL ; index++) curr = curr->next;

    if(curr == NULL) return head;

    new->next = curr->next;
    curr->next = new;
    return head;
}

// update node at index pos
void updateNodeByIndex(node *head, int pos, int val) {
    if (pos < 0) return;
    node *curr = head;

    // find node at index=pos
    for(int index = 0; index != pos && curr != NULL; index++) curr = curr->next;

    if(curr == NULL) return;

    curr->val = val;
    return;
}

// update node with val=find
void updateNodeByVal(node *head, int find, int val) {
    node *curr = head;

    while(curr->val != find) curr = curr->next;

    if(curr == NULL) return;

    curr->val = val;
    return;
}

// delete node at index=pos, return head
node *deleteNodeByIndex(node *head, int pos) {
    if (pos < 0) return head;
    if(pos == 0) {
        node *temp = head->next;
        free(head);
        return temp;
    }

    node *curr = head;

    // find node at index=pos-1
    for(int index = 0; index < pos-1 && curr != NULL; index++) curr = curr->next;

    if(curr == NULL) return head;

    node *temp = curr->next;
    curr->next = curr->next->next;
    free(temp);

    return head;
}

// delete node with val=val, return head
node *deleteNodeByVal(node *head, int val) {
    if(head->val == val) {
        node *temp = head->next;
        free(head);
        return temp;
    }

    node *curr = head;

    while(curr->next != NULL && curr->next->val != val) curr = curr->next;

    if(curr->next == NULL) return head;

    node *temp = curr->next;
    curr->next = curr->next->next;
    free(temp);

    return head;
}

// free list
node *deleteList(node *head) {
    node *curr = head;
    node *toFree = NULL;

    while(curr != NULL){
        toFree = curr;
        curr = curr->next;
        free(toFree);
    }

    return NULL;
}

int main() {
    node *head = insertHead(NULL, newNode(5));
    head = insertHead(head, newNode(12));
    head = insertHead(head, newNode(56));
    head = insertHead(head, newNode(2));
    head = insertHead(head, newNode(9));
    head = insertHead(head, newNode(90));
    head = insertHead(head, newNode(-12));

    printList(head);

    head = insertTail(head, newNode(1));
    head = insertMiddle(head, newNode(7), 3);

    printList(head);

    updateNodeByIndex(head, 4, 3);
    updateNodeByVal(head, 12, 11);

    printList(head);

    head = deleteNodeByIndex(head, 0);
    head = deleteNodeByVal(head, 56);

    printList(head);

    printf("\n");
    printf("The length of the list is %d\n", getLength(head));

    if(search(head, 99)) {
        printf("99 is in the list\n");
    } else {
        printf("99 is not in the list\n");
    }

    deleteList(head);
    return 0;
}


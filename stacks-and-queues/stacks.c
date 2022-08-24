#include <stdio.h>
#include <stdlib.h>

typedef struct stackNode {
    int val;
    struct stackNode *next;
} node;

node *newNode(int val) {
    // allocate memory
    node *new_node = (node *)calloc(1, sizeof(node));
    new_node->val = val;

    return new_node;
}

// prints stack
void printStack(node *head) {
    node *curr = head;

    while(curr != NULL){
        printf("|%d|\n", curr->val);
        printf(" |\n");
        printf(" V\n");
        curr = curr->next;
    }

    printf("NULL\n");
}

// push onto list
node *push(node *stack, node *new) {
    new->next = stack;
    return new;
}

// pop from list
node *pop(node *stack, int *popped) {
    if(stack == NULL) {
        popped = NULL;
        return stack;
    }

    node *poppedNode = stack;
    *popped = stack->val;

    stack = stack->next;
    free(poppedNode);
    return stack;
}

// free stack
node *freeStack(node *stack) {
    node *curr = stack;
    node *temp;

    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }
}

int main() {
    node *stack = push(NULL, newNode(5));
    stack = push(stack, newNode(12));
    stack = push(stack, newNode(56));
    stack = push(stack, newNode(2));
    stack = push(stack, newNode(9));
    stack = push(stack, newNode(90));
    stack = push(stack, newNode(-12));

    printStack(stack);

    int popped;

    printf("\n");

    stack = pop(stack, &popped);
    printf("The value %d was popped\n", popped);

    stack = pop(stack, &popped);
    printf("The value %d was popped\n", popped);

    printf("\n");
    printStack(stack);
    freeStack(stack);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct BST_SEARCH_TREE_NODE{
    int data;
    struct BST_SEARCH_TREE_NODE * right;
    struct BST_SEARCH_TREE_NODE * left;
}BST_Node;

BST_Node *new_BST_Node(int key){
    // Create the pointer to a new node and allocate memory
    BST_Node *new_node = NULL;
    new_node = (BST_Node *)calloc(1, sizeof(BST_Node));

    // Initialize the node
    new_node->data = key;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

// inserts new node into tree, returns root
BST_Node *BST_insert(BST_Node *root, BST_Node *new_node) {
    if(root == NULL) return new_node;

    if(new_node->data >= root->data){
        root->right = BST_insert(root->right, new_node);
    } else if(new_node->data < root->data){
        root->left = BST_insert(root->left, new_node);
    }

    return root;
}

// searches for node in tree, returns that if found or NULL if not found
BST_Node *BST_search(BST_Node *root, int key) {
    BST_Node * node = NULL;

    if(root == NULL) return NULL;

    if(key > root->data){
        node = BST_search(root->right, key);
    } else if(key < root->data){
        node = BST_search(root->left, key);
    } else{
        node = root;
    }

    return node;
}

// finds the smallest node in the tree and returns the val
int minNode (BST_Node *root) {
    BST_Node * pointer = root;

    if(root == NULL){
        printf("No tree cannot find smallest node!\n");
        return -1;
    }

    while(pointer->left != NULL){
        pointer = pointer->left;
    }

    return pointer->data;
}

// finds the largest node in the tree and returns the val
int maxNode (BST_Node *root) {
    BST_Node * pointer = root;

    if(root == NULL){
        printf("No tree cannot find smallest node!\n");
        return -1;
    }

    while(pointer->right != NULL){
        pointer = pointer->right;
    }

    return pointer->data;
}

// deletes a node with data=key, replace with successor
BST_Node *BST_delete(BST_Node *root, int key) {
    BST_Node * node;
    int min;
    if(root == NULL) return NULL;

    if(key > root->data){
        root->right = BST_delete(root->right, key);
    } else if(key < root->data){
        root->left = BST_delete(root->left, key);
    } else{
        if(root->right == NULL){
            node = root->left;
            free(root);
            return node;
        } else if(root->left == NULL){
            node = root->right;
            free(root);
            return node;
        } else{
            min = minNode(root->right);
            root = BST_delete(root, min);
            root->data = min;
            return root;
        }
    }

    return root;
}

// prints the tree
void print_tree(BST_Node *root, int indent){
    if(root != NULL){
        //print right subtree
        if(root -> right != NULL){
            print_tree(root -> right, indent + 4);
            for(int i = 0; i <= indent + 2; i++){
                printf(" ");
            }
            printf("/\n");
        }
        //print root
        for(int i = 0; i <= indent; i++){
            printf(" ");
        }
        printf("%d\n", root -> data);
        //print left subtree
        if(root -> left != NULL){
            for(int i = 0; i <= indent + 2; i++){
                printf(" ");
            }
            printf("\\\n");
            print_tree(root -> left, indent + 4);
        }
    }
}

// performs in order traversal of tree
void print_in_order(BST_Node * root){
    if(root == NULL) return;

    print_in_order(root->left);
    printf("Node at %p is %d\n", root, root->data);
    print_in_order(root->right);
}

// performs preorder traversal of tree
void preorder_traversal(BST_Node * root){
    if(root == NULL) return;

    printf("Node at %p is %d\n", root, root->data);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

// performs postorder traversal of tree
void postorder_traversal(BST_Node * root){
    if(root == NULL) return;

    postorder_traversal(root->left);
    postorder_traversal(root->right);
    printf("Node at %p is %d\n", root, root->data);
}

// deletes the tree
BST_Node * delete_bst(BST_Node * root){
    if(root == NULL) return NULL;

    delete_bst(root->left);
    delete_bst(root->right);
    printf("Freeing %d at %p\n", root->data, root);
    free(root);
}

// gets the heigh of the tree
int get_height(BST_Node* node){
    if (node == NULL)
        return 0;
    else {
        /* compute the depth of each subtree */
        int lDepth = get_height(node->left);
        int rDepth = get_height(node->right);

        /* use the larger one */
        if (lDepth > rDepth) return (lDepth + 1);
        else return (rDepth + 1);
    }
}

// finds the largest internal node in the tree (internal meaning the node has a child)
int biggest_internal(BST_Node * t, int past_data){
    int max;
    if(t != NULL){
            if(t->left != NULL || t->right != NULL){
            int left = biggest_internal(t->left, t->data);
            int right = biggest_internal(t->right, t->data);

            if(left > right) max = left;
            else max = right;

            if(max < t->data) max = t->data;

            return max;
        }
    }
    return past_data;
}

int main(){
    BST_Node * bst = NULL;
    BST_Node * search = NULL;
    bst = BST_insert(bst, new_BST_Node(56));
    bst = BST_insert(bst, new_BST_Node(17));
    bst = BST_insert(bst, new_BST_Node(24));
    bst = BST_insert(bst, new_BST_Node(78));
    bst = BST_insert(bst, new_BST_Node(67));
    bst = BST_insert(bst, new_BST_Node(98));
    bst = BST_insert(bst, new_BST_Node(5));
    bst = BST_insert(bst, new_BST_Node(2));
    bst = BST_insert(bst, new_BST_Node(15));
    bst = BST_insert(bst, new_BST_Node(31));
    bst = BST_insert(bst, new_BST_Node(71));

    print_tree(bst, 0);
    printf("\n");

    search = BST_search(bst, 67);
    if(search != NULL) printf("Data of %p is %d\n", search, search->data);
    search = BST_search(bst, 56);
    if(search != NULL) printf("Data of %p is %d\n", search, search->data);
    search = BST_search(bst, 71);
    if(search != NULL) printf("Data of %p is %d\n", search, search->data);

    printf("\n");
    printf("Smallest data in tree is: %d\n", minNode(bst));
    printf("The largest value is: %d\n", maxNode(bst));
    printf("The largest internal node is: %d\n", biggest_internal(bst, bst->data));
    printf("The height of the tree is: %d\n", get_height(bst));

    bst = BST_delete(bst, 56);
    printf("\n");
    print_tree(bst, 0);
    printf("\n");

    printf("IN ORDER\n\n");
    print_in_order(bst);
    printf("===================================\n");
    printf("PRE ORDER\n\n");
    preorder_traversal(bst);
    printf("===================================\n");
    printf("POST ORDER\n\n");
    postorder_traversal(bst);
    printf("===================================\n");

    delete_bst(bst);

    return 0;
}
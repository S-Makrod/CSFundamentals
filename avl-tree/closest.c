/************************************************************************
 *                       CSCB63
 *                  Assignment 1 - AVL Trees
 *                  (c) Mustafa Quraish
 *
 * This is the file which should be completed and submitted by you
 * for the assignment. Make sure you have read all the comments
 * and understood what exactly you are supposed to do before you
 * begin. A few test cases are provided in `testClosest.c`, which
 * can be run on the command line as follows:
 *
 *  $> gcc testClosest.c -o testClosest
 *  $> ./testClosest [optional testname]  (or .\testClosest.exe if on Windows)
 *
 * I strongly advise that you write more test cases yourself to see
 * if you have expected behaviour, especially on the edge cases for
 * insert(). You are free to make any reasonable design choices for
 * the implementation of the data structure as long as (1) the outputs
 * are consistent with the expected results, and (2) you meet the
 * complexity requirement. Your closestPair() function will only
 * be tested with cases where there are unique solutions.
 *
 * Mark Breakdown (out of 10):
 *  - 0 marks if the code does not pass at least 1 test case.
 *  - If the code passes at least one test case, then:
 *    - Up to 6 marks for successfully passing all the test cases
 *    - Up to 4 marks for meeting the complexity requirements for
 *        the functions as described in the comments below.
 *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define LEFT_HEAVY_BOUND 1
#define RIGHT_HEAVY_BOUND -1

/**
 * This defines the struct(ure) used to define the nodes
 * for the AVL tree we are going to use for this
 * assignment. You need to add some more fields here to be
 * able to complete the functions in order to meet the
 * complexity requirements
 */
typedef struct avl_node {
    // Stores the value (key) of this node
    int value;

    // Pointers to the children
    struct avl_node *left;
    struct avl_node *right;

    // TODO: Add the other fields you need here to complete the assignment!
    //      (Hint: You need at least 1 more field to keep balance)
    int height;
    int *a;
    int *b;
} AVLNode;

/**
 * This function allocates memory for a new node, and initializes it.
 * The allocation is already completed for you, in case you haven't used C
 * before. For future assignments this will be up to you!
 *
 * TODO: Initialize the new fields you have added
 */
AVLNode *newNode(int value) {
    AVLNode *node = calloc(sizeof(AVLNode), 1);
    if (node == NULL) {  // In case there's an error
        return NULL;
    }

    node->value = value;
    node->left = NULL;
    node->right = NULL;

    // Initialize values of the new fields here...
    node->height = 1;
    node->a = NULL;
    node->b = NULL;

    return node;
}

/**
 * @brief Get the height of a AVL Tree rooted at root
 *
 * @param root
 * @return int
 */
int getTreeHeight(AVLNode *root) {
    return (root == NULL) ? 0 : root->height;
}

/**
 * @brief Get the balance factor of the AVL Tree rooted at root
 *
 * @param root
 * @return int
 */
int getTreeBalanceFactor(AVLNode *root) {
    return (root == NULL) ? 0 : (getTreeHeight(root->left) - getTreeHeight(root->right));
}

/**
 * @brief Update the height values of the nodes in the tree
 *
 * @param root
 * @return AVLNode*
 */
AVLNode *updateHeight(AVLNode *root) {
    int h1 = 0, h2 = 0;

    if (root->left != NULL) {
        h1 = getTreeHeight(root->left->left);
        h2 = getTreeHeight(root->left->right);
        root->left->height = 1 + ((h1 > h2) ? h1 : h2);
    }

    if (root->right != NULL) {
        h1 = getTreeHeight(root->right->left);
        h2 = getTreeHeight(root->right->right);
        root->right->height = 1 + ((h1 > h2) ? h1 : h2);
    }

    h1 = getTreeHeight(root->left);
    h2 = getTreeHeight(root->right);
    root->height = 1 + ((h1 > h2) ? h1 : h2);

    return root;
}

/**
 * @brief Get the absolute difference between two numbers
 *
 * @param val1
 * @param val2
 * @return int
 */
int absDifference(int val1, int val2) {
    int val = val1 - val2;
    return (val < 0) ? val*-1 : val;
}

/**
 * @brief Returns the closest pair in the left subtree, using the fact that the closest pair in the tree rooted at root and its left
 * subtree will be either the root and the predecessor or the closest pair in the left child
 *
 * @param root
 * @param a
 * @param b
 */
void closestPairInLeftSubtree(AVLNode *root, int *a, int *b) {
    // get the predecessor
    AVLNode *p = root->left;
    while (p->right != NULL) p = p->right;

    // if left child has no closest pair then set the closest pair to root and predecessor
    if (root->left->a == NULL || root->left->b == NULL) {
        *a = p->value;
        *b = root->value;
    } else {
        // if left child has closest pair, compare with root and predecessor then take smaller pair
        int v1 = absDifference(root->value, p->value);
        int v2 = absDifference(*(root->left->a), *(root->left->b));

        if (v1 < v2) {
            *a = p->value;
            *b = root->value;
        } else {
            *a = *(root->left->a);
            *b = *(root->left->b);
        }
    }
}

/**
 * @brief Returns the closest pair in the right subtree, using the fact that the closest pair in the tree rooted at root and its right
 * subtree will be either the root and the successor or the closest pair in the right child
 *
 * @param root
 * @param a
 * @param b
 */
void closestPairInRightSubtree(AVLNode *root, int *a, int *b) {
    // get the successor
    AVLNode *s = root->right;
    while (s->left != NULL) s = s->left;

    // if right child has no closest pair then set the closest pair to root and successor
    if (root->right->a == NULL || root->right->b == NULL) {
        *a = root->value;
        *b = s->value;
    } else {
        // if right child has closest pair, compare with root and successor then take smaller pair
        int v1 = absDifference(root->value, s->value);
        int v2 = absDifference(*(root->right->a), *(root->right->b));

        if (v1 < v2) {
            *a = root->value;
            *b = s->value;
        } else {
            *a = *(root->right->a);
            *b = *(root->right->b);
        }
    }
}

/**
 * @brief Updates the closest pair in a tree rooted at root
 *
 * @param root
 * @return AVLNode*
 */
AVLNode *getClosestPair(AVLNode *root) {
    // if there are no children
    if (root->height == 1) {
        // reset a and b since after a rotation this could have a value
        if (root->a != NULL && root->b != NULL) {
            free(root->a);
            free(root->b);
        }
        root->a = NULL;
        root->b = NULL;
        return root;
    }

    // if children then a closest pair must exist so create space in memory if not done already
    if (root->a == NULL && root->b == NULL) {
        root->a = (int *)calloc(sizeof(int), 1);
        root->b = (int *)calloc(sizeof(int), 1);
    }

    int a1, b1, a2, b2;

    // if left tree is null then there is no check in left tree
    if (root->left == NULL) {
        closestPairInRightSubtree(root, &a1, &b1);
        *(root->a) = a1;
        *(root->b) = b1;
    // if right tree is null then no need to check in right tree
    } else if (root->right == NULL) {
        closestPairInLeftSubtree(root, &a1, &b1);
        *(root->a) = a1;
        *(root->b) = b1;
    // check both subtrees since they are not null
    } else {
        closestPairInLeftSubtree(root, &a1, &b1);
        closestPairInRightSubtree(root, &a2, &b2);

        // check which pair is closer and set a and b to that
        int v1 = absDifference(a1, b1);
        int v2 = absDifference(a2, b2);

        if (v1 < v2) {
            *(root->a) = a1;
            *(root->b) = b1;
        } else {
            *(root->a) = a2;
            *(root->b) = b2;
        }
    }

    return root;
}

/**
 * @brief Left rotate of the AVL tree
 *
 * @param root
 * @return AVLNode*
 */
AVLNode *leftRotation(AVLNode *root) {
    AVLNode *temp = root;

    // rotation
    root = root->right;
    temp->right = root->left;
    root->left = temp;

    // update height after rotation
    root = updateHeight(root);

    // update closest pair in left child since the left child was modified
    root->left = getClosestPair(root->left);

    // update closest pair in toor since root was modified
    return getClosestPair(root);
}

/**
 * @brief Right rotate of the AVL tree
 *
 * @param root
 * @return AVLNode*
 */
AVLNode *rightRotation(AVLNode *root) {
    AVLNode *temp = root;

    // rotation
    root = root->left;
    temp->left = root->right;
    root->right = temp;

    // update height after rotation
    root = updateHeight(root);

    // update closest pair in right child since the right child was modified
    root->right = getClosestPair(root->right);

    // update closest pair in toor since root was modified
    return getClosestPair(root);
}

/**
 * This function is supposed to insert a new node with the give value into the
 * tree rooted at `root` (a valid AVL tree, or NULL)
 *
 *  NOTE: `value` is a positive integer in the range 1 - 1,000,000 (inclusive)
 *       The upper bound here only exists to potentially help make the
 *                implementation of edge cases easier.
 *
 *  TODO:
 *  - Make a node with the value and insert it into the tree
 *  - Make sure the tree is balanced (AVL property is satisfied)
 *  - Return the *head* of the new tree (A balance might change this!)
 *  - Make sure the function runs in O(log(n)) time (n = number of nodes)
 *
 * If the value is already in the tree, do nothing and just return the root.
 * You do not need to print an error message.
 *
 * ----
 *
 * An example call to this function is given below. Note how the
 * caller is responsible for updating the root of the tree:
 *
 *  AVLNod *root = (... some tree is initialized ...);
 *  root = insert(root, 5); // Update the root!
 */
AVLNode *insert(AVLNode *root, int value) {
    if (root == NULL) return newNode(value);

    // will not do anything if the value is already in the tree
    if (root->value == value) return root;

    if (value < root->value) root->left = insert(root->left, value);
    else root->right = insert(root->right, value);

    int b = getTreeBalanceFactor(root);

    // tree is right heavy
    if (b < RIGHT_HEAVY_BOUND) {
        // if the right child balance factor has a different sign than the root we need a double rotation
        if (getTreeBalanceFactor(root->right) >= LEFT_HEAVY_BOUND) {
            root->right = rightRotation(root->right);
            root = leftRotation(root);
        } else {
            root = leftRotation(root);
        }
    // tree is left heavy
    } else if (b > LEFT_HEAVY_BOUND) {
        // if the left child balance factor has a different sign than the root we need a double rotation
        if (getTreeBalanceFactor(root->left) <= RIGHT_HEAVY_BOUND) {
            root->left = leftRotation(root->left);
            root = rightRotation(root);
        } else {
            root = rightRotation(root);
        }
    // tree is balanced
    } else {
        // still update height and closest pair since node was inserted
        root = updateHeight(root);
        root = getClosestPair(root);
    }

    return root;
}

/**
 * This function returns the closest pair of points in the tree rooted
 * at `root`. You can assume there are at least 2 values already in the
 * tree. Since you cannot return multiple values in C, for this function
 * we will be using pointers to return the pair. In particular, you need
 * to set the values for the two closest points in the locations pointed
 * to by `a` and `b`. For example, if the closest pair of points is
 * `10` and `11`, your code should have something like this:
 *
 *   (*a) = 10 // This sets the value at the address `a` to 10
 *   (*b) = 11 // This sets the value at the address `b` to 11
 *
 * NOTE: Make sure `(*a)` stores the smaller of the two values, and
 *                 `(*b)` stores the greater of the two values.
 *
 * NOTE: The test cases will have a unique solution, don't worry about
 *        multiple closest pairs here.
 *
 *
 * TODO: Complete this function to return the correct closest pair.
 *       Your function should not be any slower than O(log(n)), but if
 *       you are smart about it you can do it in constant time.
 */
void closestPair(AVLNode *root, int *a, int *b) {
    (*a) = *(root->a);
    (*b) = *(root->b);
    return;
}

/**
 * This function deletes the nodes and frees all memory used for nodes in it.
 * Returns NULL
 */
AVLNode *delete(AVLNode *root) {
    if(root == NULL) return NULL;

    delete(root->left);
    delete(root->right);

    if (root->a != NULL) free(root->a);
    if (root->b != NULL) free(root->b);
    free(root);

    return NULL;
}

/******************************************************************************
 * QUERY() and DELETE() are not part for this assignment, but I recommend you
 * try to implement them on your own time to make sure you understand how AVL
 * trees work.
 *
 *                              End of Assignment 1
 *****************************************************************************/
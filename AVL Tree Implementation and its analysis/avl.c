#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    int data;
    struct node* leftChild;
    struct node* rightChild;
    struct node* parent;
    int height;
} node_t;

// Define the AVL data structure
typedef struct AVL {
    node_t* root;
    int size;
} AVL_t;

void traverse(node_t* node) {
    if (node == NULL) {
        printf("null ");
        return;
    }
    printf("%d ", node->data);
    traverse(node->leftChild);
    traverse(node->rightChild);
}

void printTree(node_t* node){
    printf("The AVL is: ");
    traverse(node);
    printf("\n");
}

// Create a node
node_t* makeNode(int data) {
    //reserve heap space for node
    node_t* newNode = (node_t*)malloc(sizeof(node_t));
    if (newNode == NULL) {
        return NULL;
    }
    //initial values for the node
    newNode->data = data;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    newNode->parent = NULL;
    newNode->height = 1;
    return newNode;
}

//Create a tree
AVL_t* makeAVL(){
    //reserve space for the tree
    AVL_t* newTree = (AVL_t*)malloc(sizeof(AVL_t));
    if (newTree == NULL) {
        return NULL;
    }
    //initial values for the AVL
    newTree->root = NULL;
    newTree->size = 0;
    return newTree;
}

//free nodes
void freeNode(node_t* node) {
    if (node == NULL) {
        return;
    }
    free(node);
}

// Return larger number
int max(int a, int b) {
  return (a > b) ? a : b;
}

// Get the height of given node
int height(node_t* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Calculate the balance number
int getBalance(node_t* node){
    if (node == NULL)
        return 0;
    return height(node->leftChild) - height(node->rightChild);
}

// Update height of node and it's ancestors
void heightUpdate(node_t* node){
    node->height = max(height(node->leftChild), height(node->rightChild)) + 1;
    if (node->parent != NULL){
        heightUpdate(node->parent);
    }
}

/*
RIGHT ROTATION of a given node
      X                Y
     /       ->         \
    Y                    X
1. If old_child has a right subtree, assign node as the parent of 
the left subtree of old_child.
2. Make old_child as the parent of node.
*/
node_t* rightRotate(node_t* x){
    node_t* y = x->leftChild;
    node_t* y_right = y->rightChild;

    // Make old as new parent of x
    y->rightChild = x;
    y->parent = x->parent;
    x->parent = y;
    // Assign old's right to x
    x->leftChild = y_right;
    if (y_right != NULL)
        y_right->parent = x;

    // Update height
    x->height = max(height(x->leftChild), height(x->rightChild)) + 1;
    y->height = max(height(y->leftChild), height(y->rightChild)) + 1;

    return y;
}

/* 
LEFT ROTATION of a given node
    X                   Y
     \       ->        /
      Y               X
1. If y has a left subtree, assign x as the parent of 
the left subtree of y.
2. Make y as the parent of node.
*/
node_t* leftRotate(node_t* x){
    node_t* y = x->rightChild;
    node_t* y_left = y->leftChild;

    // Make old_child as the parent of node
    y->leftChild = x;
    y->parent = x->parent;
    x->parent = y;
    // Assign node as the parent to old_child's left
    x->rightChild = y_left;
    if (y_left != NULL)
        y_left->parent = x;

    // Update height of changed node
    x->height = max(height(x->leftChild), height(x->rightChild)) + 1;
    y->height = max(height(y->leftChild), height(y->rightChild)) + 1;

    return y;
}

// Get the balance index of a given node
int balanceIndex(node_t* node){
    if (node == NULL)
        return 0;
    return height(node->leftChild) - height(node->rightChild);
}



// Insert a node to a tree
void insert(AVL_t* tree, int value){
    if (tree == NULL ){
        printf("No tree object");
        return;
    }

    node_t* newNode = makeNode(value);
    // Insert a node without balancing
    if(tree->root == NULL){
        tree->root = newNode;
        tree->size++;
    } else {
        bool inserted = false;
        node_t* temp = tree->root;
        while (inserted == false) {
            // If insert value is smaller than node
            if (value < temp->data) {
                // If node has no left child
                // Asign new node as left child
                if (temp->leftChild == NULL) {
                    temp->leftChild = newNode;
                    newNode->parent = temp;
                    tree->size++;
                    inserted = true;
                } else {
                    // Update temp as the left child
                    // until finds a node without left child
                    temp = temp->leftChild;
                }
            } else if (value > temp->data){
                // If node has no right child
                if (temp->rightChild == NULL){
                    temp->rightChild = newNode;
                    newNode->parent = temp;
                    tree->size++;
                    inserted = true;
                } else {
                    temp = temp->rightChild;
                }
            } else {
                printf("Value already exists.");
                return;
            }
        }

        // Update height
        heightUpdate(temp);

        // Balance check
        while(temp != NULL){

            heightUpdate(temp);
            int balance = getBalance(temp);

            /*
            Situation 1:
                    Z                       Y
                   /                       / \
                  Y    Z right rotate ->  X   Z
                /
               X
            Here, the balance for Z is 2, and new insert X is smaller than Y
            */
            if (balance > 1 && value < temp->leftChild->data){
                // temp = rightRotate(temp);
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    temp_parent->leftChild = rightRotate(temp);
                } else {
                    rightRotate(temp);
                }
            }

            /*
            Situation 2:
                    Z                      Z                        Y
                   /                      /                        / \
                  X   X left rotate ->   Y   Z right rotate ->    X   Z
                   \                    /
                    Y                  X
            Here, the balance for Z is 2, and new insert Y is larger than X
            */
            else if (balance > 1 && value > temp->leftChild->data){
                temp->leftChild = leftRotate(temp->leftChild);
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    temp_parent->leftChild = rightRotate(temp);
                } else {
                    rightRotate(temp);
                }
            }

            /*
            Situation 3:
            Symatric to situation 1
            Here, the balance for Z is -2, and new insert X is larger than Y
            */
            else if (balance < -1 && value > temp->rightChild->data){
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    temp_parent->rightChild = leftRotate(temp);
                } else {
                    leftRotate(temp);
                }
            }

            /*
            Situation 4:
            Symatric to situation 2
            Here, the balance for Z is -2, and new insert X is smaller than Y
            */
            else if (balance < -1 && value < temp->rightChild->data){
                temp->rightChild = rightRotate(temp->rightChild);
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    temp_parent->rightChild = leftRotate(temp);
                } else {
                    leftRotate(temp);
                }
            } 

            if (temp->parent == NULL){
                tree->root = temp;
            }

            // Continue to check parent node
            temp = temp->parent;
        }
    }
}

// // Delete a node to a tree
// AVL_t* delete(AVL_t* tree, int value){

// }


int main(){

    AVL_t* tree = makeAVL();
    insert(tree, 11);
    insert(tree, 21);
    insert(tree, 13);
    insert(tree, 53);
    insert(tree, 33);
    insert(tree, 61);
    insert(tree, 8);
    printTree(tree->root);
    insert(tree, 9);
    printTree(tree->root);
}
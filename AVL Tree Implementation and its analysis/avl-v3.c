#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 10000

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
        printf("Node is null...\n");
        return;
    }

    if (node->leftChild == NULL) { //most left end of tree
        printf("%d ", node->data);
        if (node->rightChild != NULL) {
            traverse(node->rightChild);
        }
        
    } else { //there is a left child
       traverse(node->leftChild);
       printf("%d ", node->data);
       if (node->rightChild != NULL) {
           traverse(node->rightChild);
       }
    }
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
            // printf("balance: %d\n", balance);
            // printf("temp: %d\n", temp->data);

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
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    if (temp_parent->data > temp->data){
                        temp_parent->leftChild = rightRotate(temp);
                    } else {
                        temp_parent->rightChild = rightRotate(temp);
                    }
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
                    if (temp_parent->data > temp->data){
                        temp_parent->leftChild = rightRotate(temp);
                    } else {
                        temp_parent->rightChild = rightRotate(temp);
                    }
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
                    if (temp_parent->data > temp->data){
                        // printf("temp data: %d\n", temp->data);
                        // printf("temp parent data: %d\n", temp_parent->data);
                        temp_parent->leftChild = leftRotate(temp);
                    } else {
                        temp_parent->rightChild = leftRotate(temp);
                    }
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
                    if (temp_parent->data > temp->data){
                        temp_parent->leftChild = leftRotate(temp);
                    } else {
                        temp_parent->rightChild = leftRotate(temp);
                    }
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

//to find a num in a avl tree
node_t* find(AVL_t* tree, int num) {
    //return NULL if the tree does not exist or the tree does not have a root
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }

    //if tree is not NULL
    node_t* found = NULL;
    node_t* temp = tree->root;
    while (found == NULL) { //while the num looking for has not been found yet
        if (num == temp->data) {
            //if the num looking for equals to the the data of this node
            found = temp; //found the node!
        } else if (num < temp->data) { //num is smaller than the data in current node(temp)
            if (temp->leftChild != NULL) { 
                temp = temp->leftChild; //move to the left child
            } else { //if there is no left child, reach the left end
                return found; //still found == NULL
            }
        } else { //num is bigger than the data in current node(temp)
            if (temp->rightChild != NULL) {
                temp = temp->rightChild; //move to the right child
            } else { //if there is no right child, react the right end
                return found; //still found == NULL
            }
        }
    }
    //return found: found = the node if it is found, otherwise found is NULL
    if (found == NULL) {
        printf("%d not found in this tree", num);
    }
    return found;
}

// // Delete a node to a tree
void delete(AVL_t* tree, int num) {
    //if the num to be deleted is not in the avl tree
    if (find(tree, num) == NULL) {
        printf("Passed in data is not in the AVL Tree!");
        return;
    }
    
    //if the num to be deleted is in the avl tree
    node_t* temp = find(tree, num);
    //if the node to be deleted has no child at all
    if (temp->leftChild == NULL && temp->rightChild == NULL) {
        if (temp->parent != NULL) { //if temp node has a parent
            //if the temp node is the left child of its parent:
            if (num < temp->parent->data) {
                temp->parent->leftChild = NULL;
            } else { //else if the temp node is the right child of its parent
                temp->parent->rightChild = NULL;
            }
        }
    // if the node to be deleted has a right child, but no left child.
    } else if (temp->leftChild == NULL && temp->rightChild != NULL) {
        if (temp->parent != NULL) {
            if (num < temp->parent->data) { //temp node is the left child of its parents
                temp->parent->leftChild = temp->rightChild;
                temp->rightChild->parent = temp->parent;
            } else { //temp node is the right child of its parents
                temp->parent->rightChild = temp->rightChild;
                temp->rightChild->parent = temp->parent;
            }
        }
    // if the node to be deleted has a left child, but no right child.
    } else if (temp->leftChild != NULL && temp->rightChild == NULL) {
        if (temp->parent != NULL) {
            if (num < temp->parent->data) { //temp node is the left child of its parents
                temp->parent->leftChild = temp->leftChild;
                temp->leftChild->parent = temp->parent;
            } else { //temp node is the right child of its parents
                temp->parent->rightChild = temp->leftChild;
                temp->leftChild->parent = temp->parent;
            }
        }
    //if the node to be deleted has both left child and a right child
    } else if (temp->leftChild != NULL && temp->rightChild != NULL) {
        if (temp->parent != NULL) {
            if (num < temp->parent->data) { //temp node is the left child of its parent
                node_t* leastBiggerNode = temp->rightChild;
                while (leastBiggerNode->leftChild != NULL) {
                    leastBiggerNode = leastBiggerNode->leftChild;
                }
                delete(tree, leastBiggerNode->data);
                temp->data = leastBiggerNode->data;
            } else { //temp node is the right child of its parents
                node_t* leastSmallerNode = temp->leftChild;
                while (leastSmallerNode ->rightChild != NULL) {
                    leastSmallerNode = leastSmallerNode->rightChild;
                }
                delete(tree, leastSmallerNode->data);
                temp->data = leastSmallerNode->data;
            }
        } else { //when the temp node is the root
            node_t* leastBiggerNode = temp->rightChild;
            while (leastBiggerNode->leftChild != NULL) {
                leastBiggerNode = leastBiggerNode->leftChild;
            }
            delete(tree, leastBiggerNode->data);
            temp->data = leastBiggerNode->data;
        }
    //tree->size--;
    }

    // Update height
        heightUpdate(temp);

        // Balance check
        while(temp != NULL){

            heightUpdate(temp);
            int balance = getBalance(temp);

            /*
            Situation 1: Right Rotate
                    Z                       Y
                   /                       / \
                  Y    Z right rotate ->  X   Z
                /
               X
            Here, the balance for Z is 2, and new insert X is smaller than Y
            */
            if (balance > 1 && getBalance(temp->leftChild) >= 0){
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    // temp_parent->leftChild = rightRotate(temp);
                    if (temp_parent->data > temp->data){
                        temp_parent->leftChild = rightRotate(temp);
                    } else {
                        temp_parent->rightChild = rightRotate(temp);
                    }
                } else {
                    rightRotate(temp);
                }
            }

            /*
            Situation 2: Left - Right Rotate
                    Z                      Z                        Y
                   /                      /                        / \
                  X   X left rotate ->   Y   Z right rotate ->    X   Z
                   \                    /
                    Y                  X
            Here, the balance for Z is 2, and new insert Y is larger than X
            */
            else if (balance > 1 && getBalance(temp->leftChild) < 0){
                temp->leftChild = leftRotate(temp->leftChild);
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    if (temp_parent->data > temp->data){
                        temp_parent->leftChild = rightRotate(temp);
                    } else {
                        temp_parent->rightChild = rightRotate(temp);
                    }
                } else {
                    rightRotate(temp);
                }
            }

            /*
            Situation 3: Left Rotate
            Symatric to situation 1
            Here, the balance for Z is -2, and new insert X is larger than Y
            */
            else if (balance < -1 && getBalance(temp->rightChild) <= 0){
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    if (temp_parent->data > temp->data){
                        temp_parent->leftChild = leftRotate(temp);
                    } else {
                        temp_parent->rightChild = leftRotate(temp);
                    }
                } else {
                    leftRotate(temp);
                }
            }

            /*
            Situation 4: Right - Left Rotate
            Symatric to situation 2
            Here, the balance for Z is -2, and new insert X is smaller than Y
            */
            else if (balance < -1 && getBalance(temp->rightChild) > 0){
                temp->rightChild = rightRotate(temp->rightChild);
                if (temp->parent != NULL){
                    node_t* temp_parent = temp->parent;
                    //temp_parent->rightChild = leftRotate(temp);
                    if (temp_parent->data > temp->data){
                        temp_parent->leftChild = leftRotate(temp);
                    } else {
                        temp_parent->rightChild = leftRotate(temp);
                    }
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
        free(temp);
    }

void freeTree(node_t* node) {
    if (node == NULL) {
        return;
    }
    freeTree(node->leftChild);
    freeTree(node->rightChild);
    free(node);
}

void freeAVL(AVL_t* tree){
    free(tree);
}

//test test used to insert 10000 numbers (from 1 to 10000)
int testInsert1() {
    AVL_t* avlTree1 = makeAVL();
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    for (int i = 0; i < SIZE; i++) {
        insert(avlTree1, i);
    }
    printTree(avlTree1->root);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used is: %f\n", cpu_time_used);
    freeTree(avlTree1->root);
    freeAVL(avlTree1);
    return 0;
}

//test test used to insert 10000 numbers (from 10000 to 1)
int testInsert2() {
    AVL_t* avlTree2 = makeAVL();
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    for (int i = SIZE; i > 0; i--) {
        insert(avlTree2, i);
    }
    printTree(avlTree2->root);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used is: %f\n", cpu_time_used);
    freeTree(avlTree2->root);
    freeAVL(avlTree2);
    return 0;
}

//test test used to insert 10000 numbers (random permutation)
int testInsert3() {
    //generate random permutation of 10000 numbers, within range of [0,10000]
    AVL_t* avlTree3 = makeAVL();
    clock_t start, end;
    double cpu_time_used;
    int array[SIZE];

    int i;
    for (i = 0; i < SIZE; i ++){
        array[i] = i;
    }
    for (i = SIZE - 1; i > 1; i--) {
        int j = rand() % i;
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }

    start = clock();
    for (i = 0; i < SIZE; i ++){
        insert(avlTree3, array[i]);
    }
    printTree(avlTree3->root);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used is: %f\n", cpu_time_used);
    freeTree(avlTree3->root);
    freeAVL(avlTree3);
    return 0;
}

int testSearch() {
    AVL_t* avlTree4 = makeAVL();
    clock_t start, end;
    double cpu_time_used;
    int array[SIZE];

    int i;
    for (i = 0; i < SIZE; i ++){
        array[i] = i;
    }
    for (i = SIZE - 1; i > 1; i--) {
        int j = rand() % i;
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
    for (i = 0; i < SIZE; i ++){
        insert(avlTree4, array[i]);
    }

    start = clock();
    for (int i = 0; i < SIZE; i++) {
        find(avlTree4, i);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used is: %f\n", cpu_time_used);
    freeTree(avlTree4->root);
    freeAVL(avlTree4);
    return 0;
}

int main(){

    printf("\n<AVL Tree> sorting permutation of 1 to 100000...\n");
    for (int i = 0; i < 10; i++) {
        testInsert1();
    }
    printf("\n<AVL Tree> sorting permutation of 100000 to 1...\n");
    for (int i = 0; i < 10; i++) {
        testInsert2();
    }
    printf("\n<AVL Tree> sorting random permutations of 100000 integers...\n");
    for (int i = 0; i < 10; i++) {
        testInsert3();
    }

    printf("\n<AVL Tree> searching random elements for 100000 times...\n");
    for (int i = 0; i < 10; i++) {
        testSearch();
    }

    // AVL_t* tree = makeAVL();
    // insert(tree, 1);
    // insert(tree, 2);
    // insert(tree, 3);
    // insert(tree, 4);
    // insert(tree, 5);
    // insert(tree, 6);
    // printTree(tree->root);

}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 10000

//define the node data structure
//a node contains data, pointer to its left child, its right child and its parent
typedef struct node {
    int data;
    struct node* leftChild;
    struct node* rightChild;
    struct node* parent;
} node_t;

//define the BST data structure
//a BST contains a size and a root node
typedef struct BST {
    node_t* root;
    int size;
} BST_t;


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
    return newNode;
}


BST_t* makeBST() {
    //reserve space for the tree
    BST_t* newTree = (BST_t*)malloc(sizeof(BST_t));
    if (newTree == NULL) {
        return NULL;
    }
    //initial values for the BST
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

//to find a num in a binary search tree
node_t* find(BST_t* tree, int num) {
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
    return found;
}

BST_t* insert(BST_t* tree, int num) {
    //create a new node with the passed in data: num
    node_t* newNode = makeNode(num);

    //check if the tree is NULL
    if (tree == NULL) {
        return NULL;
    }
    
    //if tree is not NULL
    //if this is an empty tree (root is null)
    if (tree->root == NULL) {
        tree->root = newNode;
        tree->size ++;
    } else {
        bool inserted = false;
        node_t* temp = tree->root;
        while (inserted == false) {
            if (num < temp->data) {
                if (temp->leftChild == NULL) {
                    newNode->parent = temp;
                    temp->leftChild = newNode;
                    tree->size++;
                    inserted = true;
                } else {
                    temp = temp->leftChild;
                }
            } else {
                if (temp->rightChild == NULL) {
                    newNode->parent = temp;
                    temp->rightChild = newNode;
                    tree->size++;
                    inserted = true;
                } else {
                    temp = temp->rightChild;
                }
            }
        }
    }
    return tree;
}

void delete(BST_t* tree, int num) {
    //if the num to be deleted is not in the bst
    if (find(tree, num) == NULL) {
        printf("Passed in data is not in the Binary Saerch Tree!");
        return;
    }
    
    //if the num to be deleted is in the bst
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
        }
    freeNode(temp);
    tree->size--;
    }
}

void traverse(node_t* node) {
    if (node == NULL) {
        //printf("nothing to print...\n");
        return;
    }
    //printf("%d ", node->data);
    if (node->leftChild == NULL) { //most left end of tree
        if (node->rightChild != NULL) {
            traverse(node->rightChild);
        }
        
    } else { //there is a left child
       traverse(node->leftChild);
       //printf("%d ", node->data);
       if (node->rightChild != NULL) {
           traverse(node->rightChild);
       }
    }
}

void printTree(BST_t* tree) {
    //printf("The BST is: [");
    traverse(tree->root);
    //printf("].\n");
}

void runBST() {
    BST_t* bst1 = makeBST();
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
        insert(bst1, array[i]);
    }
    printTree(bst1);
    return;
}

// int main() {
//     clock_t start, end;
//     double cpu_time_used;
//     for (int i = 0; i<10; i++) {
//         start = clock();
//         runBST();
//         end = clock();
//         cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//         printf("Time used is: %f\n", cpu_time_used);
//     }
//     return 0;
// }

void freeTree(node_t* node) {
    if (node == NULL) {
        return;
    }
    freeTree(node->leftChild);
    freeTree(node->rightChild);
    free(node);
}

void freeBST(BST_t* tree){
    free(tree);
}

//test for bst to insert 100000 numbers (from 1 to 100000)
int testInsert1() {
    BST_t* BSTTree1 = makeBST();
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    for (int i = 0; i < SIZE; i++) {
        insert(BSTTree1, i);
    }
    //printTree(BSTTree1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used is: %f\n", cpu_time_used);
    //freeTree(avlTree1->root);
    freeTree(BSTTree1->root);
    freeBST(BSTTree1);
    return 0;
}

//test test used to insert 10000 numbers (from 10000 to 1)
int testInsert2() {
    BST_t* BSTTree2 = makeBST();
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    for (int i = SIZE; i > 0; i--) {
        insert(BSTTree2, i);
    }
    printTree(BSTTree2);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used is: %f\n", cpu_time_used);
    //freeTree(avlTree1->root);
    freeTree(BSTTree2->root);
    freeBST(BSTTree2);
    return 0;
}

//test test used to insert 10000 numbers (random permutation)
int testInsert3() {
    //generate random permutation of 10000 numbers, within range of [0,10000]
    BST_t* BSTTree3 = makeBST();
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
        insert(BSTTree3, array[i]);
    }
    printTree(BSTTree3);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used is: %f\n", cpu_time_used);
    //freeTree(BSTTree3->root);
    freeTree(BSTTree3->root);
    freeBST(BSTTree3);
    return 0;
}

int testSearch() {
    BST_t* BSTTree4 = makeBST();
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
        insert(BSTTree4, array[i]);
    }

    start = clock();
    for (int i = 0; i < SIZE; i++) {
        find(BSTTree4, i);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used is: %f\n", cpu_time_used);
    //freeTree(BSTTree4->root);
    freeTree(BSTTree4->root);
    freeBST(BSTTree4);
    return 0;
}

int main(){

    printf("\n<Binary Search Tree> sorting permutation of 1 to 100000...\n");
    for (int i = 0; i < 10; i++) {
        testInsert1();
    }
    printf("\n<Binary Search Tree> sorting permutation of 100000 to 1...\n");
    for (int i = 0; i < 10; i++) {
        testInsert2();
    }
    printf("\n<Binary Search Tree> sorting random permutations of 100000 integers...\n");
    for (int i = 0; i < 10; i++) {
        testInsert3();
    }

    printf("\n<Binary Search Tree> searching random elements for 100000 times...\n");
    for (int i = 0; i < 10; i++) {
        testSearch();
    }
}
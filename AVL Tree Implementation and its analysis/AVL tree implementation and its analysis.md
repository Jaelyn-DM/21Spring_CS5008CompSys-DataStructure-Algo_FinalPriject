# AVL Tree Analysis

Group: Echo, Mingming, Jaelyn, Zhe

# General Idea about AVL Tree

### **What is AVL Tree?**

AVL tree is a self-balancing Binary Search Tree (BST) where the difference between heights of left and right subtrees cannot be more than one for all nodes.

### **Why AVL Trees?**

Most of the BST operations (e.g., search, max, min, insert, delete.. etc) take O(h) time where h is the height of the BST. The cost of these operations may become O(n) for a skewed Binary tree. If we make sure that the height of the tree remains O(Logn) after every insertion and deletion, then we can guarantee an upper bound of O(Logn) for all these operations. The height of an AVL tree is always O(Logn) where n is the number of nodes in the tree.

### **How to be Self-Balanced?**

In order to update the balance factors of all nodes, first, observe that all nodes requiring correction from child to parent along the path of the inserted node. 

The retracing can stop if the balance factor becomes 0 implying that the height of that subtree remains unchanged. If the balance factor becomes ±1 then the height of the subtree increases by one and the retracing needs to continue. If the balance factor temporarily becomes ±2, this has to be repaired by an appropriate rotation after which the subtree has the same height as before (and its root the balance factor 0).

# AVL Tree Code Implementation

### Data Structure and General Ideas

Our implementation of an AVL tree is based on two type of data structures: tree and node. A tree contains a root node and an integer that indicates the size of the tree. A tree node in an AVL tree contains an integer that is the data that is stored in this node, an integer that indicates the height of this node, and three node pointers that point to its left child node, its right child node, and its parent node if there is any. 

In our implementation, for the node data structure specifically, the alternative way of doing it would be to not include the pointer points to the parent node. The difference between these two ways is similar to the difference between a singly linked list and a doubly linked list - there is certain sacrifice to the operation complexity caused by the extra pointer, however when the height of the tree, h, goes large, it will become handy to traverse backward and locate one's parent node when needed, instead of finding the parent by traversing all over again from the root. 

AVL tree is a height-self-balanced kind of binary search tree, so an AVL tree is capable of achieving all functions that a binary search tree can achieve. In our implementation, the AVL tree contains the following functions: insert, delete, search, traverse, print, and rebalance. Here is the code structure of our AVL tree implementation. 

```c
typedef struct node {
    int data;
    struct node* leftChild;
    struct node* rightChild;
    struct node* parent;
    int height;
} node_t;

typedef struct AVL {
    node_t* root;
    int size;
} AVL_t;

void traverse(node_t* node)
void printTree(node_t* node)
int max(int a, int b)
int height(node_t* node)
int getBalance(node_t* node)
void heightUpdate(node_t* node)
node_t* leftRotate(node_t* x)
node_t* rightRotate(node_t* x)
int balanceIndex(node_t* node)
void insert(AVL_t* tree, int value)
node_t* find(AVL_t* tree, int num)
```

### Inserting and Rebalancing

In general, there are several steps to complete the entire insert process:

First, we need a common binary search tree insert process to just insert the node into correct position.

Then, get the balancing number of the new insert node and that of all its ancestors.

Third, we should start rebalance to tree if needed.

- Right Rotate: balance for z is 2, and new insert x is smaller than y

In order to figure out the logic of rotation and self-balancing, we have to divide inserting nodes into different situations. In the image below, if the new insert x is smaller than y, it will go to the left of y and will cause the extra height of z's left subtree. However, since y is the middle number, we can make this structure more balanced by moving y to the top to be the parent of x and y. We call this treatment as Right Rotate, and we know that this treatment happens when the left subtree is 2 higher than the right subtree (balancing number is 2 or bigger than 1).

When implementing Right Rotate, there are some important points needed to make clear:

1. Take care of y's right child if it exists. Since y has to become the new top, its child needs to be linked to the old top z. Since y is on the left of z, its child will also be left child of z.
2. Be careful to link z's parent with y. If z was a right child, so will be y. If z was a root, so will be y. Otherwise y will lose connect with original tree.

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Mind_Map_-_Frame_1.jpg](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Mind_Map_-_Frame_1.jpg)

---

- Left Rotate: balance for z is -2, and new insert x is larger than y

After Right Rotate, Left Rotate will have similar logic to implement. Only need to be careful with the symmetric situation of left and right.

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Mind_Map_-_Frame_2.jpg](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Mind_Map_-_Frame_2.jpg)

---

- Left-Right Rotate: balance for z is 2, and new insert y is larger than x

Besides single rotation, there are two more situations with a new insert x to be the top. In the first step, we can see that y, x, and x's child will form the same situation of Left Rotate. After that, we can see another shape of rotating z with a Right Rotate.

1. Be careful of which node is being rotate with, and which rotate it should be done.
2. Again, connect z's parent to new insert x

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Mind_Map_-_Frame_3.jpg](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Mind_Map_-_Frame_3.jpg)

---

- Right-Left Rotate: balance for z is -2, and new insert y is smaller than x.

Again, this will be symmetric to Left-Right Rotate.

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Mind_Map_-_Frame_4.jpg](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Mind_Map_-_Frame_4.jpg)

For our experience of translating logic above into c language, we met many challenges and choices about the process.

1. Since we decided to build a tree struct, we will also pass the entire tree into the insert function. The benefit of building a tree struct is that we can have multiple tree objects for different testing purposes. However, our hands are tied if we pass the tree into function since we cannot make a change of its size or traverse its nodes (if we can make a change of size, we can only focus on the small part of nodes that need to be rotated). To solve that problem, we introduced a while loop to travel through the leaf node all the way above to check the balance number and make corresponding changes. 
2. Always check before making any moves. Be careful of linking two nodes (especially when link a's parent to b's child) because they might not exist. Save a temporary object for some nodes before making a rotation on that. Sometimes, the program will not get the parent if the node is rotated after.
3. Some tradeoffs: If we pass the node (say root node) to the function, then each insert checking can be done by possibly recursions since each time we are doing the same treatment to each node. Recursions will also help avoid extra conditions for non-existing parent or child node (set a stop point if parent or child is null).

---

### Deleting function

To delete a node in an AVL tree, our implementation firstly utilizes the search function to find and return the node that contains the data that we are looking for to delete. After finding the node to be deleted, the delete function would run differently depending on the location of the node. The general idea here is that we cannot delete a node directly from a tree unless it is a leaf child, because we have to deal with the subtrees that are connected with the node to be deleted before actually deleting the data. Therefore, we deal with the following four situation:

1. when the node to be deleted is a lead node, that is this node has no child node at all.

    This is the most straight-forward situation to deal with because the node to be deleted is a leaf node and it has no child. Deleting this node directly would not affect the rest of tree, so we can just delete or "take off" this node from the AVL tree. We do this by reassigning the pointer of its parent node. If the node to be deleted is a left child to its parent node, then we reassign the left child node pointer of the parent node to NULL; else if the node to be deleted is a right child to its parent node, then we reassign the right child node pointer of the parent to NULL. After this step, all we need to do is to free the memory space of the deleted node.

2. when the node to be deleted has a left child, but no right child. 

    In this situation since the node to be deleted has a pointer points to its left child node, we cannot delete this node directly. Deleting this node directly would affect the left subtree of this node, so we have to ensure the left subtree of this node is connected with the parent node of this node before deleting or "taking off" this node from the AVL tree. We do this by reassigning the pointer of its parent node and the pointer of its left child.

    If the node to be deleted is a left child to its parent node, then we reassign the left child node pointer of the parent node to the left child node of the this node; else if the node to be deleted is a right child to its parent node, then we reassign the right child node pointer of the parent to the left child node of the this node. Then we reassign the parent node pointer of the left child of this node to the parent node of this node. After this step, all we need to do is to free the memory space of the deleted node.

3. when the node to be deleted has a right child, but no left child.

    In this situation since the node to be deleted has a pointer points to its right child node, we cannot delete this node directly. Deleting this node directly would affect the right subtree of this node, so we have to ensure the right subtree of this node is connected with the parent node of this node before deleting or "taking off" this node from the AVL tree. We do this by reassigning the pointer of its parent node and the pointer of its right child. 

    If the node to be deleted is a left child to its parent node, then we reassign the left child node pointer of the parent node to the right child node of the this node; else if the node to be deleted is a right child to its parent node, then we reassign the right child node pointer of the parent to the right child node of the this node. Then we reassign the parent node pointer of the right child of this node to the parent node of this node. After this step, all we need to do is to free the memory space of the deleted node.

4. when the node to be deleted has both a left child and a right child. 

    In this situation where the node to be deleted has both a pointer points to its left child node and a pointer points to its right child node, we cannot delete this node directly. Deleting this node directly would affect both the subtrees of this node, so we have to swap this node with the one leaf node that is the most close to itself before deleting this node. In this way, this node would be swapped to the leaf level and deleting the node them would be easy and straightforward. On the other hand, the node that is swapped with the node to be deleted would be the successor of this node in this tree. The successor node of this node can be either the one holds the least bigger value, that is the most left node of the right subtree of the node to be deleted, or the one holds the least smaller value comparing to the value the node to be deleted holds, that is the most right node if the left subtree of the node to be deleted.

    If the node to be deleted is not a the root of the tree, which means there is a parent of this node, then there are two scenarios - if the node to be deleted is a left child of its parent, we chose to swap it with the least bigger node as its successor, and if the node to be deleted is the right child of its parent, we chose to swap it with the least smaller node as its successor. After swapping we call the delete function again to delete the new leaf node. If the node to be deleted is the root of the tree, that means there is no parent node of this node. In this scenarios, no swap needed but we would need to put the successor of this node to be the new root of this AVL tree. 

    After swapping, the next step is to free the deleted node and update the height for each of the node in the tree. And based on the result of the height updates, AVL tree will be rebalanced if needed. The logic for how to rebalance the tree follows the same idea of how to rotate the tree nodes  when inserting new elements into the tree.  

---

### **Searching function:**

In an AVL tree, the elements are located in the tree the same way as to how they are located in a binary search tree. So the search function in an AVL tree follows the same logic. To search for a number in an AVL tree, the search function starts to search for the passed in number from the root of the tree, if the passed in number equals the value in this node, then the number that we are looking for is found and the function returns this node; otherwise, if the passed in number is smaller than this node, the search function will go to the left child of this node for comparison; if the passed in number is larger than the root, the search function will go to the right child of this node for comparison. Once the searching function reaches to the left end or the right end of the tree, meaning that the number that we are searching for is larger than the value of most right node or smaller than the value of the most left node, searching function terminates and return NULL as the number we are searching for is not found. 

---

### **Traversing function:**

As AVL tree is a kind of binary search tree that is self-balanced on its height, we implemented the traverse function in the same way to the traverse function in the regular binary search tree. As a node is always greater than its left child and is smaller than its right child, and we would like to print out the nodes in ascending order, we implemented the inorder traversal our traverse function. In this function, we recursively call the function itself on the left child node, if there is any, before printing the current node and calling the function itself on the right child node, if there is any.

# Comparison with other algorithms:

## AVL Tree versus Binary Search Tree

Since AVL tree is an height-self-balanced kind of binary search tree, we believe there are some similarities between these two in terms of run time complexity and memory space complexity. We would like to compare the performance between an AVL tree and a Binary Search tree in the following aspects:

- Sorting time complexity comparison in regular case scenarios
- Sorting time complexity comparison in worst case scenarios
- Searching time complexity comparison

### AVL v.s. BST: Sorting time complexity in regular-case scenarios

The tree-sort algorithm applies to both AVL tree and Binary Search tree when it comes to sort n elements. Both AVL tree and Binary Search tree sorts n elements by inserting elements from the array one by one into the tree structure, and both the tree follows the property of a binary search tree such that a new node would be the left subtree of a node contains only the nodes with data that is smaller than the data this node holds, and the right subtree of a node contains only the nodes with data that is larger than the data that this node holds, while both the left and right subtree of each node must also satisfy this binary search tree property. By this rule, in either tree, each time to insert a new node into the tree, the new node is implemented at the proper position. By the time that all elements have been passed in and inserted into the tree structure, all nodes are at proper position and all elements are sorted by traversing the tree in inorder traversal order. 

The only difference between an AVL tree and a Binary Search tree is that each time after inserting a new node into the tree, the Binary Search tree completes this insertion step and moves on to the next while the AVL tree would track the height of each node and rebalance the tree by rotating certain nodes if necessary. This operation can make differences in two ways:

- it increases the operation complexity for each time of inserting new element, which could possibly result in longer run time when rotating happens quite often. This makes AVL tree sort take longer than a BST tree.
- it ensures the tree's height is balanced, or minimized, as a binary search tree, which could possibly shorten the traversing time from root to the leaf level of the tree. This makes AVL tree sort take shorter than a BST tree.

As the two effect mentioned above, we would like to find out which of the above effect would dominate the performance of an AVL tree sort implementation. So we implemented the following test: let both AVL tree to insert and sort a random permutation of 100,000 integers. The results are shown as below:

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-28_at_10.35.50_AM.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-28_at_10.35.50_AM.png)

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-28_at_10.01.37_AM.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-28_at_10.01.37_AM.png)

From the result above we can tell that to build an AVL tree, it takes longer time than takes to build a binary search tree. The average time to build an AVL tree with random permutation of 100,000 integers is 0.240, while to build a BST tree in the same situation takes 0.038 seconds. The result makes sense constantly rotating and rebalancing the height of the tree after each step of inserting a new elements definitely increase the operation complexity in building the AVL tree. Even though the balanced height shorten the time for traversing, it is offset by the added rotating and rebalancing operation complexity. 

### AVL v.s. BST: Sorting time complexity in worst-case scenarios

We believe that the inserting order will affect the performance of AVL tree, and it will reveal the difference from it with binary search tree. In theory, the AVL tree should have significant contribution in shorten the time of building a tree when the binary search tree is extremely unbalanced. Therefore we decided to conduct an experiment for the worst case scenarios (extremely unbalanced situation for binary search trees) for comparison. 

We conduct the experiment on sorting ascending and descending numbers from 1 to 100,000. These two situations will create an extreme case for binary search tree. In these cases, the binary search tree is more like a singly linked list and lost the strength as trees - it keeps inserting new nodes to the tree as the left child (or right child) to the only leaf node. In these worst cases, when n goes larger and larger, it would be very much time consuming when it comes to traverse from the root to the leaf node to insert new element. The result of this experiment are shown below:

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-29_at_10.13.37_AM.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-29_at_10.13.37_AM.png)

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-29_at_10.14.58_AM.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-29_at_10.14.58_AM.png)

As we can see from the result above, in the worst case scenarios for binary search tree, AVL tree is way much better on sorting. The time it takes to build an AVL tree in these extremely unbalanced cases is under 0.2 seconds on average, while to build a binary search tree in these extremely unbalanced cases it takes over 16 seconds on average. This result is shocking but it is within our expectation. The reason is that AVL tree will only take O(logn) to traverse on leaf nodes, while Binary Search tree will take O(n). When n goes large to 100,000, the difference shows significantly. 

### AVL v.s. BST: Searching time complexity comparison

We believe one of the biggest advantage for an AVL tree, comparing to a Binary Search tree, is that it shortens the runtime for searching for an element in a tree, as the height of an AVL tree is balanced. To prove our expectations, we started an experiment to test the difference in runtime between searching in an AVL tree and in a Binary Search tree. 

Binary Search tree is relatively fast in searching for an element thanks to its unique tree structure, it search an element in O(logn) time, and we expect searching in AVL tree to run even faster as the height (=logn) in AVL tree is lower than or equal to the height in Binary Search tree. On the other hand, search function in tree structure is a relatively simple operation - all it does is to traverse from the root to the leaf level potentially, and it might terminate somewhere in between. To compare the difference between two fast operations, the result shows only subtle difference. Therefore we have decided to increase the experimental size to 1 million. 

We first created an AVL tree and a Binary Search tree by inserting 100,000 integers from a random permutation of 1 to 100,000, without repeating element. Then we timed the searching function run time in both AVL tree and Binary Search tree for a random element for 1 million times. We repeated the experiments for 10 times and the results are shown below. From the results we can tell that the same searching function runs in AVL tree takes 0.012 seconds in average while it takes 0.049 seconds in average in a Binary Search tree. Searching function's runtime in AVL tree is about 4 times shorter comparing to the one in Binary Search tree. As analyzed above, the result from this experiment is aligned with our expectation - AVL tree structure shortens the searching function's runtime in binary search tree structure. 

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-29_at_8.38.02_AM.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-29_at_8.38.02_AM.png)

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-29_at_8.38.22_AM.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/Screen_Shot_2021-04-29_at_8.38.22_AM.png)

---

## AVL Tree Sort versus Other Sorting Algorithms

### AVL v.s. Insertion Sort: Sorting time complexity comparison

Besides the tree itself, we can also compare AVL's sorting with another sorting algorithm. Take one of the basic sorting methods Insertion Sort as an example. For n=100,000, we can see that Insertion Sort takes way more time than the AVL tree. One of the main reasons is that the time complexity of Insertion Sort is O(n^2), which is much larger than that of the AVL tree. Therefore, we can tell that for a large number of n, AVL tree will have better performance than Insertion Sort or possibly selection sort and bubble sort.

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_9.27.12.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_9.27.12.png)

---

### AVL v.s. Merge Sort: Sorting time complexity comparison

If we test merge sort for n=100,000 as well, we can see that the performance of merge sort is outstanding. It's even much faster than AVL tree if using random permutation. Even though Merge Sort also has O(nlogn) complexity, it requires also fewer steps and spaces than AVL tree since it's concentrating on sorting but AVL tree spent a lot of time taking care of insertion and rebalancing.

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_9.27.37.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_9.27.37.png)

---

## AVL Tree Sort v.s. Other Sorting Algorithms in **Space Complexity:**

### AVL Tree:

We used valgrind to see the heap usage of different algorithm. When we run test on the AVL tree, we met some challenges about leak memory. There is 0 lost bytes but 148 bytes still reachable. We found that every malloced objects are freed and there is no lost bytes. When we trace back to the reachable bytes, we found that most of them are happening in printf function (c language is interesting). 

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_8.41.41.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_8.41.41.png)

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_8.41.53.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_8.41.53.png)

After figuring out the reachable leak memory, we noticed that the total heap usage is 400,219 allocs, which is a huge number.

---

### Binary Search Tree:

When we start to test on binary search tree, we found that the total heap usage is the same as AVL tree. That makes sense because both tree will alloc n nodes to store all the data an other information. 

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_9.04.57.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_9.04.57.png)

---

### Insertion Sort & Merge Sort

For Insertion Sort and Merge Sort, the heap usage are exactly the same for both of them. There is only 179 allocs which is much less than that of trees. The reason is that Insertion and Merge don't need to alloc anything but only use array to perform sorting.

Some people may confused about why the heap usage are the same, since merge sort have additional array created to store the sorting number. The reason is that created instance variables are not stored on heap but on stack. Therefore, the additional array will not increase the heap usage. 

![AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_9.11.07.png](AVL%20Tree%20Analysis%20ac42f155ae4e405ead830e913a1676c2/2021-04-27_9.11.07.png)

# Summary

As an AVL tree is balanced on its height, it brings several advantages (or disadvantages) when it comes to implement sorting and searching functions. Based on the analysis and the result of experiments above, there are certain tradeoffs between the sorting runtime and the balanced height. Usually the frequent rotating and rebalancing step increases the operation complexity within each step so it costs longer runtime in sorting function. However, in extreme unbalanced case scenarios, the benefits of a balanced height shows. An AVL tree improves the performance and shortens the runtime significantly in sorting large amount of elements in worst-case or extremely unbalanced scenarios. Meanwhile another benefit brought by a balanced height is that it shortens the search time comparing to a regular binary search tree. On the other hand the space complexity is definitely not a shortcoming for an AVL tree, it takes exactly the same space to a binary search tree. 

Therefore overall, we believe the AVL tree is a perfect option to offset the defeat of a binary search tree in sorting large amount of unbalanced numbers. The height self-balancing feature  perfectly prevents the situation that a tree would be extremely unbalanced and inefficient. On the other hand, AVL tree is a great option as a tree data structure to store data for searching. Its balanced height ensures the efficiency in searching. In a sort-rare-but-search-often situation, AVL tree performs the best.
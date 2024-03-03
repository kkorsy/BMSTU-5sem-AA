#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include <iostream>
using namespace std;

struct NodeAvl {
    int key;
    NodeAvl* left;
    NodeAvl* right;
    int height;
};

NodeAvl* createAvlNode(int key);
int getAvlHeight(NodeAvl* node);
int max(int a, int b);
NodeAvl* rightRotate(NodeAvl* y);
NodeAvl* leftRotate(NodeAvl* x);
int getBalance(NodeAvl* node);
NodeAvl* insertAvl(NodeAvl* node, int key);
NodeAvl* minValueNode(NodeAvl* node);
NodeAvl* deleteNode(NodeAvl* root, int key);
bool searchAvl(NodeAvl* root, int key, int& count_compares);
void printAvlTree(NodeAvl* root);

NodeAvl* generate_avl_tree(int size);

#endif
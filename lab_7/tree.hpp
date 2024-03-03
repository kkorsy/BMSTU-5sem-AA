#ifndef __TREE_H__
#define __TREE_H__

#include <iostream>

struct Node {
    int data;
    Node* left;
    Node* right;
};

Node* createNode(int value);
Node* insert(Node* root, int value);
bool search(Node* root, int value, int& count_compares);
Node* minValueNode(Node* node);
Node* deleteNode(Node* root, int value);
void printTree(Node* root);
int height(Node* node);

Node* generate_tree(int size);

#endif
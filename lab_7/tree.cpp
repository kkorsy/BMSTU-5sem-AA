#include "tree.hpp"
#include <queue>
#include <iomanip>
using namespace std;

// Функция для создания нового узла
Node* createNode(int value) {
    Node* newNode = new Node();
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Функция для добавления нового узла в дерево
Node* insert(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }

    return root;
}

// Функция для поиска узла со значением value в дереве
bool search(Node* root, int value, int& count_compares) {
    if (root == NULL) {
        return false;
    } else if (root->data == value) {
        count_compares++;
        return true;
    } else if (value <= root->data) {
        count_compares++;
        return search(root->left, value, count_compares);
    } else {
        count_compares++;
        return search(root->right, value, count_compares);
    }
}

// Функция для нахождения узла с минимальным значением (левым узлом)
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Функция для удаления узла с заданным значением из дерева
Node* deleteNode(Node* root, int value) {
    if (root == NULL) {
        return root;
    }

    if (value < root->data) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->data) {
        root->right = deleteNode(root->right, value);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        Node* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

// Вспомогательная функция, которая возвращает высоту дерева
int height(Node* node) {
    if (node == nullptr) {
        return 0;
    } else {
        int left_height = height(node->left);
        int right_height = height(node->right);

        return (left_height > right_height) ? left_height + 1 : right_height + 1;
    }
}

// Функция для вывода двоичного дерева с линиями
void printTree(Node* root) {
    int h = height(root);
    int i;
    int current_level = 1;

    // Очередь для узлов
    std::queue<Node*> q;
    q.push(root);

    while (current_level <= h) {
        int nodes_in_level = q.size();
        for (i = 0; i < nodes_in_level; i++) {
            Node* temp = q.front();
            q.pop();
            if (temp) {
                std::cout << std::setw(4) << temp->data;
                if (temp->left) {
                    q.push(temp->left);
                } else {
                    q.push(nullptr);
                }
                if (temp->right) {
                    q.push(temp->right);
                } else {
                    q.push(nullptr);
                }
            } else {
                std::cout << std::setw(4) << " ";
                q.push(nullptr);
                q.push(nullptr);
            }
            if (i != nodes_in_level - 1) {
                std::cout << std::setw(4) << "---";
            }
        }
        std::cout << std::endl;
        current_level++;
    }
}

Node* generate_tree(int size)
{
    srand(time(0));
    Node* root = nullptr;
    int temp = 0;
    for (int i = 0; i < size; ++i)
    {
        int r = rand() % size;
        if (!search(root, r, temp))
            root = insert(root, r);
        else
            i--;
    }

    return root;
}

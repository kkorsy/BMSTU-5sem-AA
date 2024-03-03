#include "avl_tree.hpp"
#include <queue>
#include <iomanip>

// Функция для создания нового узла
NodeAvl* createAvlNode(int key) {
    NodeAvl* newNode = new NodeAvl();
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;  // Начальная высота нового узла равна 1
    return newNode;
}

// Функция для получения высоты узла (если узел пуст, возвращает 0)
int getAvlHeight(NodeAvl* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

// Функция для получения максимального значения из двух целых чисел
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Функция для выполнения правого поворота поддерева с корнем y
NodeAvl* rightRotate(NodeAvl* y) {
    NodeAvl* x = y->left;
    NodeAvl* T2 = x->right;

    // Проворачиваем
    x->right = y;
    y->left = T2;

    // Обновляем высоты узлов
    y->height = max(getAvlHeight(y->left), getAvlHeight(y->right)) + 1;
    x->height = max(getAvlHeight(x->left), getAvlHeight(x->right)) + 1;

    return x;
}

// Функция для выполнения левого поворота поддерева с корнем x
NodeAvl* leftRotate(NodeAvl* x) {
    NodeAvl* y = x->right;
    NodeAvl* T2 = y->left;

    // Проворачиваем
    y->left = x;
    x->right = T2;

    // Обновляем высоты узлов
    x->height = max(getAvlHeight(x->left), getAvlHeight(x->right)) + 1;
    y->height = max(getAvlHeight(y->left), getAvlHeight(y->right)) + 1;

    return y;
}

// Функция для получения баланса узла
int getBalance(NodeAvl* node) {
    if (node == NULL) {
        return 0;
    }
    return getAvlHeight(node->left) - getAvlHeight(node->right);
}

// Функция для добавления нового узла в дерево
NodeAvl* insertAvl(NodeAvl* node, int key) {
    if (node == NULL) {
        return createAvlNode(key);
    }

    if (key < node->key) {
        node->left = insertAvl(node->left, key);
    } else if (key > node->key) {
        node->right = insertAvl(node->right, key);
    } else {
        // Дублирующиеся ключи не разрешены в AVL дереве
        return node;
    }

    // Обновляем высоту текущего узла
    node->height = 1 + max(getAvlHeight(node->left), getAvlHeight(node->right));

    // Получаем баланс узла, чтобы проверить, нарушается ли он свойство баланса AVL дерева
    int balance = getBalance(node);

    // Если узел нарушает баланс, то есть 4 возможные случая вставки (LL, LR, RL, RR)
    // Не сбалансированый узел
    if (balance > 1 && key < node->left->key) {
        return rightRotate(node);
    }
    if (balance < -1 && key > node->right->key) {
        return leftRotate(node);
    }
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Узел сбалансирован
    return node;
}

// Функция для нахождения узла с минимальным значением (левым узлом)
NodeAvl* minValueNode(NodeAvl* node) {
    NodeAvl* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Функция для удаления узла с заданным значением из дерева
NodeAvl* deleteNode(NodeAvl* root, int key) {
    if (root == NULL) {
        return root;
    }

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            NodeAvl* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            delete temp;
        } else {
            NodeAvl* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL) {
        return root;
    }

    // Обновляем высоту текущего узла
    root->height = 1 + max(getAvlHeight(root->left), getAvlHeight(root->right));

    // Получаем баланс узла
    int balance = getBalance(root);

    // Если узел нарушает баланс, то есть 4 возможные случая удаления (LL, LR, RL, RR)
    // Несбалансированый узел
    if (balance > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Функция для поиска узла со значением key в дереве
bool searchAvl(NodeAvl* root, int key, int& count_compares) {
    if (root == NULL) {
        return false;
    } else if (root->key == key) {
        count_compares++;
        return true;
    } else if (key < root->key) {
        count_compares++;
        return searchAvl(root->left, key, count_compares);
    } else {
        count_compares++;
        return searchAvl(root->right, key, count_compares);
    }
}

// Функция для вывода двоичного дерева с линиями
void printAvlTree(NodeAvl* root) {
    int h = getAvlHeight(root);
    int i;
    int current_level = 1;

    // Очередь для узлов
    std::queue<NodeAvl*> q;
    q.push(root);

    while (current_level <= h) {
        int nodes_in_level = q.size();
        for (i = 0; i < nodes_in_level; i++) {
            NodeAvl* temp = q.front();
            q.pop();
            if (temp) {
                std::cout << std::setw(4) << temp->key;
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

NodeAvl* generate_avl_tree(int size)
{
    srand(time(0));
    NodeAvl* root = nullptr;
    int temp = 0;
    for (int i = 0; i < size; ++i)
    {
        int r = rand() % size;
        if (!searchAvl(root, r, temp))
            root = insertAvl(root, r);
        else
            i--;
    }

    return root;
}

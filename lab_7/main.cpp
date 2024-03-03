#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "tree.hpp"
#include "avl_tree.hpp"

using namespace std;

int menu();
void compare_measure();
void write_to_file(int size, int cmp_max, int cmp_min);

int main(void)
{
    int cmd = menu();
    int s, result;
    while (cmd)
    {
        switch (cmd)
        {
        case 1:
        {
            cout << "Введите размер дерева: ";
            cin >> s;
            Node* root = generate_tree(s);

            printTree(root);

            cout << "Введите искомый элемент: ";
            cin >> s;
            result = 0;
            if (search(root, s, result))
                cout << "Элемент найден" << endl;
            else
                cout << "Элемент НЕ найден" << endl;
            cout << "Количество сравнений: " << result << endl;
            break;
        }
        case 2:
        {
            cout << "Введите размер дерева: ";
            cin >> s;
            NodeAvl* root_avl = generate_avl_tree(s);

            printAvlTree(root_avl);

            cout << "Введите искомый элемент: ";
            cin >> s;
            result = 0;
            if (searchAvl(root_avl, s, result))
                cout << "Элемент найден" << endl;
            else
                cout << "Элемент НЕ найден" << endl;
            cout << "Количество сравнений: " << result << endl;
            break;
        }
        case 3:
        {
            compare_measure();
            break;
        }
        default:
        {
            cout << "Введена неверная команда" << endl;
            break;
        }
        }
        cmd = menu();
    }
    

}

void write_to_file(ofstream& f, int size, int cmp_max, int cmp_min)
{
    f << setw(16) << size;
    f << setw(10) << cmp_max;
    f << setw(10) << cmp_min << endl;
}

void compare_measure()
{
    ofstream f("binaryResults.txt", std::ios::out);
    f << setw(16) << "Кол-во элементов";
    f << setw(10) << "Хс";
    f << setw(10) << "Лс" << endl;
    vector<int> tree_sizes = {128, 256, 512, 1024, 2048};
    int cmp_min = 0, cmp_max = 0;
    int result;
    // binary tree
    for (auto s : tree_sizes)
    {
        Node* root = generate_tree(s);
        for (int i = -1; i < s + 1; i++)
        {
            result = 0;
            search(root, i, result);
            cmp_max = (result > cmp_max) ? result : cmp_max;
            cmp_min = (result < cmp_min) ? result : cmp_min;
        }
        write_to_file(f, s, cmp_max, cmp_min);
        cmp_max = 0; cmp_min = 0;
    }
    f.close();
    // avl tree
    ofstream g("avlResults.txt", std::ios::out);
    g << setw(16) << "Кол-во элементов";
    g << setw(10) << "Хс";
    g << setw(10) << "Лс" << endl;
    cmp_min = 0; cmp_max = 0;
    for (auto s : tree_sizes)
    {
        NodeAvl* root = generate_avl_tree(s);
        for (int i = -1; i < s + 1; i++)
        {
            result = 0;
            searchAvl(root, i, result);
            cmp_max = (result > cmp_max) ? result : cmp_max;
            cmp_min = (result < cmp_min) ? result : cmp_min;
        }
        write_to_file(g, s, cmp_max, cmp_min);
        cmp_min = 0; cmp_max = 0;
    }
    g.close();
}

int menu()
{
    cout << "1. Поиск в несбалансированном дереве\n2. Поиск в сбалансированном дереве\n3. Замерить время\n\n0. Выход\n" << endl;
    cout << "Выберите пункт меню: ";
    int cmd;
    cin >> cmd;
    return cmd;
}

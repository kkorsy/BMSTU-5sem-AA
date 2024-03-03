#include <iostream>
#include <fstream>
#include <thread>
#include <set>
#include <vector>
#include <mutex>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;
using std::string;

void KMP(int start_pos, string text, string pattern, std::set<int> &result);
std::ifstream get_file();
void print_res(std::set<int> &res);
int menu();

int main(void)
{
    std::streampos size;
    std::ifstream in;
    char* text;
    string pattern;
    std::set<int> result;

    int cmd = menu();
    while (cmd)
    {
        switch (cmd)
        {
        case 1:
            in = get_file();
            if (in.is_open())
            {
                size = in.tellg();
                text = new char[size];
                in.seekg(0, std::ios::beg);
                in.read(text, size);
                in.close();
                cout << "Введите искомую подстроку: ";
                cin >> pattern;
                KMP(0, text, pattern, result);
                print_res(result);
            }
            else {
                cout << "Не удалось открыть файл" << endl;
            }
            break;
        
        default:
            break;
        }
        cmd = menu();
        result.clear();
    }

    return 0;
}


void KMP(int start_pos, string text, string pattern, std::set<int> &result)
{
    int m = text.length();
    int n = pattern.length();
    if (n == 0)
    {
        result.insert(0);
        return;
    }
    if (m < n)
        return;
 
    // next[i] сохраняет индекс следующего лучшего частичного совпадения
    int next[n + 1];
    for (int i = 0; i < n + 1; i++) {
        next[i] = 0;
    }
 
    for (int i = 1; i < n; i++)
    {
        int j = next[i];
        while (j > 0 && pattern[j] != pattern[i]) {
            j = next[j];
        }
        if (j > 0 || pattern[j] == pattern[i]) {
            next[i + 1] = j + 1;
        }
    }
 
    for (int i = 0, j = 0; i < m; i++)
    {
        if (text[i] == pattern[j])
        {
            if (++j == n) {
                result.insert(start_pos + i - j + 1);
            }
        }
        else if (j > 0)
        {
            j = next[j];
            i--;    // так как `i` будет увеличен на следующей итерации
        }
    }
}

void print_res(std::set<int> &res)
{
    if (res.size() == 0)
        cout << "Подстрока не найдена" << endl;
    else {
        int i = 1;
        for (auto it = res.begin(); it != res.end(); ++it, ++i)
            cout << "Найденный индекс " << i << ": " << *it << endl;
    }
}

std::ifstream get_file()
{
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;
    std::ifstream in(filename, std::ios::in|std::ios::binary|std::ios::ate);

    return in;
}

int menu()
{
    cout << "Меню: \n1. Поиск подстроки\n0. Выход\n\n";
    cout << "Выберите пункт меню: ";
    int cmd;
    cin >> cmd;
    return cmd;
}
 
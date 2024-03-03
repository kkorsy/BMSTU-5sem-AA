#include <iostream>
#include <fstream>
using std::cin;
using std::cout;
using std::endl;
using std::string;

void KMP(string text, string pattern)
{
    bool found = false;                                 // 1
    int m = text.length();                              // 2
    int n = pattern.length();                           // 3
    if (n == 0)                                         // 4
    {
        cout << "Найденный индекс: 0" << endl;          // 5
        return;
    }
    if (m < n)                                          // 6
    {
        cout << "Подстрока не найдена" << endl;         // 7
        return;
    }

    int next[n + 1];                                    // 8
    for (int i = 0; i < n + 1; i++) {                   // 9
        next[i] = 0;                                    // 10
    }
 
    for (int i = 1; i < n; i++)                         // 11
    {
        int j = next[i];                                // 12
        while (j > 0 && pattern[j] != pattern[i]) {     // 13
            j = next[j];                                // 14
        }
        if (j > 0 || pattern[j] == pattern[i]) {        // 15
            next[i + 1] = j + 1;                        // 16
        }
    }
    // ...
}

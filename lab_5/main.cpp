#include <iostream>
#include <fstream>
#include <chrono>
#include "conveyor.h"

int menu();
void time_measure();

int main(void)
{
    int cmd = menu();
    while (cmd)
    {
        switch (cmd)
        {
        case 1:
            int req_cnt, str_len;
            cout << "Введите количество заявок: ";
            cin >> req_cnt;
            cout << "Введите длину искомых подстрок: ";
            cin >> str_len;
            conveyor_start(req_cnt, str_len);
            break;
        case 2:
            time_measure();
            break;
        
        default:
            break;
        }
        cmd = menu();
    }

    return 0;
}

void time_measure()
{
    std::ofstream out("latex_table.txt");
    int str_len = 10;
    double sum = 0;
    for (int i = 100; i < 1001; i += 100)
    {
        for (int n = 0; n < 10; n++)
        {
            auto t1 = std::chrono::system_clock::now();
            conveyor_start(i, str_len);
            auto t2 = std::chrono::system_clock::now();
            sum += (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
        }
        sum /= 100; sum /= 1e6;
        out << i << " & " << sum << " & " << "\\\\ \\hline" << endl;
    }
}

int menu()
{
    cout << "1. Конвейерная обработка\n2. Замерить время\n\n0. Выход\n" << endl;
    cout << "Выберите пункт меню: ";
    int cmd;
    cin >> cmd;
    return cmd;
}
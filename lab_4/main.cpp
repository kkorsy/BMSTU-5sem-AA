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

void KMP_par(int n, string text, string pattern, std::set<int> &result);
void KMP(int start_pos, string text, string pattern, std::set<int> &result);
void print_res(std::set<int> &res);
void time_measure_threads(string text);
void time_measure_length();
int menu();

std::mutex mutex;

int main(void)
{
    string text;
    std::ifstream in("str.txt");
    if (in.is_open())
    {
        in >> text;
        in.close();
    }
    string pattern;
    std::set<int> result;

    int cmd = menu();
    while (cmd)
    {
        switch (cmd)
        {
        case 1:
            cout << "Введите искомую подстроку: ";
            cin >> pattern;
            KMP(0, text, pattern, result);
            print_res(result);
            break;
        case 2:
            cout << "Введите искомую подстроку: ";
            cin >> pattern;
            int n;
            cout << "Введите количество потоков: ";
            cin >> n;
            KMP_par(n, text, pattern, result);
            print_res(result);
            break;
        case 3:
            // time_measure_threads(text);
            time_measure_length();
            break;
        
        default:
            break;
        }
        cmd = menu();
        result.clear();
    }

    
 
    return 0;
}

void KMP_par(int n, string text, string pattern, std::set<int> &result)
{
    std::vector<std::thread> vect;
    int k = (int)(text.length() / n);
    int q = pattern.length(); 
    for (int i = 0; i < n; ++i)
    {
        int start_pos, count;
        if (i == 0) {
            start_pos = 0; count = k + q;
        }
        else if (i == n - 1) {
            start_pos = i * k - q; count = k;
        }
        else {
            start_pos = i * k - q; count = k + q;
        }
        if (start_pos < 0)
            start_pos = 0;
        if (start_pos + count > text.length()) {
            start_pos = i * k - q; count = k;
        }

        string part_text = text.substr(start_pos, count);
        std::thread t(KMP, start_pos, part_text, pattern, std::ref(result));
        vect.push_back(std::move(t));
    }

    for (int i = 0; i < n; ++i)
        vect[i].join();
}

void KMP(int start_pos, string text, string pattern, std::set<int> &result)
{
    int m = text.length();
    int n = pattern.length();
    if (n == 0)
    {
        mutex.lock();
        result.insert(0);
        mutex.unlock();
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
                mutex.lock();
                result.insert(start_pos + i - j + 1);
                mutex.unlock();
            }
        }
        else if (j > 0)
        {
            j = next[j];
            i--;    // так как `i` будет увеличен на следующей итерации
        }
    }
}

string generate_str(int length)
{
    static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result;
    result.resize(length);

    srand(time(NULL));
    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];

    return result;
}

void time_measure_length()
{
    std::ofstream out("latex_table2.txt");
    if (out.is_open())
    {
        string pattern = generate_str(25);
        for (int n = 100000; n < 2000001; n += 100000)
        {
            double sumPar = 0, sumPosl = 0;
            string text = generate_str(n);
            for (int i = 0; i < 100; ++i)
            {
                std::set<int> result;
                auto t1 = std::chrono::system_clock::now();
                KMP_par(32, text, pattern, result);
                auto t2 = std::chrono::system_clock::now();
                sumPar += (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());

                result.clear();
                t1 = std::chrono::system_clock::now();
                KMP(0, text, pattern, result);
                t2 = std::chrono::system_clock::now();
                sumPosl += (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
            }
            sumPar /= 100; sumPosl /= 100;
            sumPar /= 1e6; sumPosl /= 1e6;
            out << n << " & " << sumPosl << " & " << sumPar << "\\\\ \\hline" << endl;
        }

        out.close();
    }
}

void time_measure_threads(string text)
{
    std::ofstream out("latex_table.txt");
    if (out.is_open())
    {
        std::vector<int> threads_num = {1, 2, 4, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96};
        string pattern = "ABGqgs";
        for (int n = 0; n < threads_num.size(); ++n)
        {
            double sum = 0;
            for (int i = 0; i < 100; ++i)
            {
                std::set<int> result;
                auto t1 = std::chrono::system_clock::now();
                KMP_par(threads_num[n], text, pattern, result);
                auto t2 = std::chrono::system_clock::now();
                sum += (std::chrono::duration_cast<std::chrono::nanoseconds>
                    (t2 - t1).count());
            }
            sum /= 100;
            sum /= 1e6;
            out << threads_num[n] << " & " << sum << "\\\\ \\hline" << endl;
        }

        out.close();
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

int menu()
{
    cout << "Меню: \n1. Последовательный поиск \n2. Параллельный поиск \n3. Замерить время выполнения\n\n0. Выход\n\n";
    cout << "Выберите пункт меню: ";
    int cmd;
    cin >> cmd;
    return cmd;
}
 
#include "conveyor.h"
# define NO_OF_CHARS 256

std::mutex m;

void conveyor_start(int req_cnt, int str_len)
{
    srand(time(NULL));
    std::ofstream log("log.txt");
    if (!log.is_open()) {
        cout << "Не удалось открыть log файл" << endl;
        return;
    }
    log << std::right << std::setw(8) << "id req";
    log << std::right << std::setw(20) << "time";
    log << std::right << std::setw(11) << "stage num" << endl;

    std::vector<string> files = {"text1.c", "text2.c", "text3.c"};
    // create req_cnt requests
    std::queue<request_t> q1, q2, q3, q_ready;
    for (int i = 0; i < req_cnt; ++i) {
        request_t req = {.id = i + 1, .filename = files[rand() % files.size()], .pattern = generate_str(str_len)};
        q1.push(req);
    }
    request_t req = {.id = -1, .flag_end = true};
    q1.push(req);

    // create 3 threads
    std::vector<std::thread> th_vect;
    std::thread t1(stage1, std::ref(q1), std::ref(q2), std::ref(log));
    th_vect.push_back(std::move(t1));

    std::thread t2(stage2, std::ref(q2), std::ref(q3), std::ref(log));
    th_vect.push_back(std::move(t2));

    std::thread t3(stage3, std::ref(q3), std::ref(q_ready), std::ref(log));
    th_vect.push_back(std::move(t3));

    for (int i = 0; i < 3; ++i)
        th_vect[i].join();
 
    log_statistic(q_ready, log);
}

string generate_str(int len)
{
    static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result;
    result.resize(len);

    for (int i = 0; i < len; i++)
        result[i] = charset[rand() % charset.length()];

    return result;
}

// поиск подстроки в бинарном файле кмп
void stage1(std::queue<request_t> &q_from, std::queue<request_t> &q_to, std::ofstream &log)
{
    request_t cur_req;
    do
    {
        cur_req = q_from.front();
        q_from.pop();
        if (cur_req.flag_end)
        {
            q_to.push(cur_req);
            break;
        }

        auto t1 = std::chrono::system_clock::now();
        kmp(cur_req);
        auto t2 = std::chrono::system_clock::now();
        cur_req.t_system += (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()) / 1e6;
        cur_req.t_stage1 = (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()) / 1e6;

        // write to log: id, t_stage1, stage description
        m.lock();
        log << std::right << std::setw(8) << cur_req.id;
        log << std::right << std::setw(20) << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1e6; 
        log << std::right << std::setw(11) << "stage 1" << endl;
        m.unlock();

        cur_req.t_wait = std::chrono::system_clock::now();
        q_to.push(cur_req);
    } while (!cur_req.flag_end);
    
}

// поиск подстроки в бинарном файле бм
void stage2(std::queue<request_t> &q_from, std::queue<request_t> &q_to, std::ofstream &log)
{
    request_t cur_req;
    do
    {
        m.lock();
        bool is_empty = q_from.empty();
        m.unlock();

        if (!is_empty)
        {
            m.lock();
            cur_req = q_from.front();
            q_from.pop();
            m.unlock();
            auto t = std::chrono::system_clock::now();
            cur_req.t_queue_2 += (std::chrono::duration_cast<std::chrono::nanoseconds>(t - cur_req.t_wait).count()) / 1e6;
            if (cur_req.flag_end)
            {
                q_to.push(cur_req);
                break;
            }

            auto t1 = std::chrono::system_clock::now();
            bm(cur_req);
            auto t2 = std::chrono::system_clock::now();
            cur_req.t_system += (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()) / 1e6;
            cur_req.t_stage2 = (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()) / 1e6;

            // write to log: id, t_stage2, stage description
            m.lock();
            log << std::right << std::setw(8) << cur_req.id;
            log << std::right << std::setw(20) << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1e6; 
            log << std::right << std::setw(11) << "stage 2" << endl;
            m.unlock();

            m.lock();
            cur_req.t_wait = std::chrono::system_clock::now();
            q_to.push(cur_req);
            m.unlock();
        }
        else
            continue;

    } while (!cur_req.flag_end);
}

// дамп результатов в файл: кмп индексы, бм индексы, подстрока, имя файла
void stage3(std::queue<request_t> &q_from, std::queue<request_t> &q_to, std::ofstream &log)
{
    request_t cur_req;
    do
    {
        m.lock();
        bool is_empty = q_from.empty();
        m.unlock();

        if (!is_empty)
        {
            m.lock();
            cur_req = q_from.front();
            q_from.pop();
            m.unlock();
            auto t = std::chrono::system_clock::now();
            cur_req.t_queue_3 += (std::chrono::duration_cast<std::chrono::nanoseconds>(t - cur_req.t_wait).count()) / 1e6;
            if (cur_req.flag_end)
            {
                q_to.push(cur_req);
                break;
            }

            auto t1 = std::chrono::system_clock::now();
            dump(cur_req);
            auto t2 = std::chrono::system_clock::now();
            cur_req.t_system += (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()) / 1e6;
            cur_req.t_stage3 = (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()) / 1e6;

            // write to log: id, t_stage3, stage description
            m.lock();
            log << std::right << std::setw(8) << cur_req.id;
            log << std::right << std::setw(20) << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1e6; 
            log << std::right << std::setw(11) << "stage 3" << endl;
            m.unlock();

            m.lock();
            q_to.push(cur_req);
            m.unlock();
        }
        else
            continue;

    } while (!cur_req.flag_end);
}

string read_from_file(request_t &req)
{
    std::ifstream in(req.filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (!in.is_open())
        return string();
    std::streampos size = in.tellg();
    char *text = new char[size];
    in.seekg(0, std::ios::beg);
    in.read(text, size);
    in.close();

    return text;
}

void kmp(request_t &req)
{
    string text = read_from_file(req);
    string pattern = req.pattern;
    set<int>& result = req.kmp_indexes;

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
                result.insert(i - j + 1);
            }
        }
        else if (j > 0)
        {
            j = next[j];
            i--;    // так как `i` будет увеличен на следующей итерации
        }
    }
}

void preprocess_strong_suffix(int *shift, int *bpos, string pat, int m)
{
    int i = m, j = m + 1;
    bpos[i] = j;
 
    while(i > 0)
    {
        while(j <= m && pat[i - 1] != pat[j - 1])
        {
            if (shift[j] == 0)
                shift[j] = j - i;
 
            j = bpos[j];
        }
        i--; j--;
        bpos[i] = j; 
    }
}
 
void preprocess_case2(int *shift, int *bpos, string pat, int m)
{
    int i, j;
    j = bpos[0];
    for(i = 0; i <= m; i++)
    {
        if(shift[i] == 0)
            shift[i] = j;

        if (i == j)
            j = bpos[j];
    }
}

void bm(request_t &req)
{
    string text = read_from_file(req);
    string pattern = req.pattern;
    set<int>& result = req.bm_indexes;

    int m = pattern.length(); 
    int n = text.length(); 

    int s = 0, j;
    int bpos[m + 1], shift[m + 1];

    for(int i = 0; i < m + 1; i++) shift[i] = 0;
 
    preprocess_strong_suffix(shift, bpos, pattern, m);
    preprocess_case2(shift, bpos, pattern, m);
 
    while(s <= n - m)
    {
        j = m - 1;
        while(j >= 0 && pattern[j] == text[s+j])
            j--;
        if (j < 0)
        {
            result.insert(s);
            s += shift[0];
        }
        else
            s += shift[j + 1];
    }
}

void dump(request_t &req)
{
    std::ofstream in("dump.txt", std::ios::app);
    if (!in.is_open())
    {
        cout << "Не удалось открыть файл" << endl;
        return;
    }
    in << std::right << std::setw(10) << "id: " << req.id << endl;
    in << "substring: " << req.pattern << endl;
    in << "filename: " << req.filename << endl;
    in << std::right << std::setw(15) << "kmp indexes: ";
    for (auto it = req.kmp_indexes.begin(); it != req.kmp_indexes.end(); it++)
        in << *it << " ";
    in << endl;
    in << std::right << std::setw(15) << "bm indexes: ";
    for (auto it = req.bm_indexes.begin(); it != req.bm_indexes.end(); it++)
        in << *it << " ";
    in << endl;
    in << endl;
}

void log_statistic(std::queue<request_t> &q, std::ofstream &log)
{
    // write to log: t_min, t_med, t_avg, t_max
    int cnt = q.size();
    int median_ind = (int)(cnt / 2);

    request_t req = q.front();
    q.pop();
    // time_t t_sum = req.t_system + req.t_queue_2 + req.t_queue_3; 
    time_t t_sum_proc = req.t_stage1 + req.t_stage2 + req.t_stage3;

    time_t t_min = t_sum_proc, t_avg = t_sum_proc, t_max = t_sum_proc, t_med;
    log << endl;
    log << "Время обработки:" << endl;
    log << std::right << std::setw(15) << "t_min";
    log << std::right << std::setw(15) << "t_med";
    log << std::right << std::setw(15) << "t_avg"; 
    log << std::right << std::setw(15) << "t_max" << endl;

    int ind = 1;
    while (!q.empty())
    {
        ind++;
        req = q.front();
        q.pop();

        t_sum_proc = req.t_stage1 + req.t_stage2 + req.t_stage3;
        if (t_sum_proc < t_min && t_sum_proc != 0)
            t_min = t_sum_proc;
        if (t_sum_proc > t_max && t_sum_proc != 0)
            t_max = t_sum_proc;
        t_avg += t_sum_proc;
        if (ind == median_ind)
            t_med = t_sum_proc;
    }
    t_avg /= cnt;

    log << std::right << std::setw(15) << t_min;
    log << std::right << std::setw(15) << t_med;
    log << std::right << std::setw(15) << t_avg; 
    log << std::right << std::setw(15) << t_max << endl;
    
}

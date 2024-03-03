#ifndef __CONVEYOR_H__
#define __CONVEYOR_H__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include <chrono>

#define STR_MAX_LEN 256

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::set;

struct request 
{
    int id;

    string filename;
    string pattern;

    set<int> kmp_indexes;
    set<int> bm_indexes;

    time_t t_system = 0, t_queue_2 = 0, t_queue_3 = 0;
    time_t t_stage1 = 0, t_stage2 = 0, t_stage3 = 0;
    std::chrono::system_clock::time_point t_wait;

    bool flag_end = false;
};
using request_t = struct request;

void conveyor_start(int req_cnt, int str_len);
void stage1(std::queue<request_t> &q_from, std::queue<request_t> &q_to, std::ofstream &log);
void stage2(std::queue<request_t> &q_from, std::queue<request_t> &q_to, std::ofstream &log);
void stage3(std::queue<request_t> &q_from, std::queue<request_t> &q_to, std::ofstream &log);
void log_statistic(std::queue<request_t> &q, std::ofstream &log);
void kmp(request_t &req);
void bm(request_t &req);
void dump(request_t &req);
string generate_str(int max_len);

#endif
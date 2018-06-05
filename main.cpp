#include <iostream>
#include <future>
#include <vector>
#include <chrono>
#include <atomic>

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

unsigned long long counter(int start, int end, int a0, int d)
{
    int a_prev = a0 + (start - 1) * d;
    unsigned long long local_result = a_prev;
    for (int i = start; i < end; i++) {
        a_prev = a_prev + d;
        local_result = local_result + a_prev;
    }
    return local_result;
}

int main(int argc, char* argv[]) {
    int a0 = atoi(argv[1]);
    int d = atoi(argv[2]);
    int N = atoi(argv[3]);
    int numOfAsyncs = atoi(argv[4]);
    unsigned long long result = 0;
    std::vector<std::future<unsigned long long>> calcs;

    auto stage1_start_time = get_current_time_fenced();
    for (int thr = 0; thr < numOfAsyncs; thr++){
        calcs.push_back(std::async(std::launch::async, counter, (N * thr / numOfAsyncs) + 1, (N * (thr + 1) / numOfAsyncs), a0, d));
    }

    for (auto&& fut:calcs)
        result += fut.get();
    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - stage1_start_time;
    std::cout << "Total time: " << to_us(total_time) << std::endl;

    std::cout << "Total: " << result << std::endl;

    return 0;
}
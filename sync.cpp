#include <iostream>
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

int main(int argc, char* argv[]) {
    int a0 = atoi(argv[1]);
    int d = atoi(argv[2]);
    int N = atoi(argv[3]);
    int result = a0;
    int a_prev = a0;

    auto stage1_start_time = get_current_time_fenced();
    for (int i = 1; i < N; i++) {
        a_prev = a_prev + d;
        result = result + a_prev;
    }
    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - stage1_start_time;
    std::cout << "Total time: " << to_us(total_time) << std::endl;

    std::cout << result << std::endl;

    return 0;
}
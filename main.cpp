#include <iostream>
#include <future>
#include <vector>

int counter(int start, int end, int a0, int d)
{
    int a_prev = a0 + (start - 1) * d;
    int local_result = a_prev;
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
    int result = 0;
    std::vector<std::future<int>> calcs;

    for (int thr = 0; thr < numOfAsyncs; thr++){
        calcs.push_back(std::async(std::launch::async, counter, (N * thr / numOfAsyncs) + 1, (N * (thr + 1) / numOfAsyncs), a0, d));
    }

    for (auto&& fut:calcs)
        result += fut.get();

    std::cout << "Total: " << result << std::endl;

    return 0;
}
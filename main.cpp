#include <iostream>

int main(int argc, char* argv[]) {
    int a0 = atoi(argv[1]);
    int d = atoi(argv[2]);
    int N = atoi(argv[3]);
    int result = a0;
    int a_prev = a0;

    for (int i = 1; i < N; i++) {
        a_prev = a_prev + d;
        result = result + a_prev;
    }

    std::cout << result << std::endl;

    return 0;
}
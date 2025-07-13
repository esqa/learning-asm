#include <iostream>
#include <chrono>
#include <climits>
#include <iomanip>
#include <Windows.h>

int main()
{
    const int n = 1024;
    long long sum;

    auto calc_start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++)
    {
        sum = i;
    }

    auto calc_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> calc_duration = calc_end - calc_start;

    std::cout << std::fixed << std::setprecision(9);
    std::cout << "Sum calculation duration: " << calc_duration.count() << " seconds\n";
    std::cout << "Sum: " << sum << "\n";

    SetConsoleTitleA("calc but fast"); // i'm calling this after the main calc since it's gonna mess with the speed

    std::cout << "\nPress enter to exit";
    std::cin.get();

    return 0;
}
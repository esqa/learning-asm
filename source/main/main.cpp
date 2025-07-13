#include <iostream>
#include <chrono>
#include <climits>
#include <iomanip>
#include <Windows.h>

int __cdecl sum(int a, int b)
{
    int result{};

    __asm
    {
        mov eax, a;
        add eax, b;
        mov result, eax;
    }

    return result;
}

int main()
{
    int cool{};

    __asm
    {
        push 10;
        push 20;
        call sum;
        add esp, 8;
        mov cool, eax;
    }

    printf("Result: %d\n", cool);

    return 0;
}
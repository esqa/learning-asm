#include <iostream>
#include <chrono>
#include <climits>
#include <iomanip>
#include <Windows.h>

__declspec(naked) int __cdecl sum(int a, int b, int c)
{
    __asm
    {
        push ebp;
        mov ebp, esp;
        sub esp, 4;

        mov dword ptr [ebp - 4], 100;

        mov eax, [ebp + 8]; // 7
        add eax, [ebp + 12]; // 4
        add eax, [ebp + 16]; // 2

        add eax, [ebp - 4];

        mov esp, ebp;
        pop ebp;
        ret;
    }
}

int main()
{
    int cool{};

    __asm
    {
        push 2;
        push 4;
        push 7;

        call sum;
        add esp, 12;

        mov cool, eax;
    }

    printf("%d\n", cool);

    return 0;
}

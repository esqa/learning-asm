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
    int cool{}, con_handle{}, msg_len{};
    char buffer[32];

    // josh - let's do our calculation
    __asm
    {
        push 2;
        push 4;
        push 7;

        call sum;
        add esp, 12;

        mov cool, eax;
    }

    msg_len = sprintf(buffer, "%d\n", cool); // NOLINT I know it's deprecated but "sprintf_s" sucks

    // josh - let's call the stinky Windows API to get our handle
    __asm
    {
        push -11; // STD_OUTPUT_HANDLE
        call GetStdHandle;
        mov con_handle, eax; // josh - GetStdHandle was called, return register is eax for integers
    }

    // josh - now let's actually print, push right to left onto the stack because of __stdcall
    __asm
    {
        push 0;
        push 0;
        push msg_len;
        lea eax, buffer; // josh - get address of our buffer
        push eax;
        push con_handle;
        call WriteConsoleA; // josh - It's an __stdcall so we don't have to clean-up the stack!
    }

    return 0;
}
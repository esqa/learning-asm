#include <iostream>
#include "Windows.h"

__declspec(naked) int __cdecl sum(int a, int b, int c, int d, int skip)
{
    __asm
    {
        push ebp;
        mov ebp, esp;
        sub esp, 4;

        push edx;
        mov edx, [ebp + 24]; // josh - get our condition from args
        mov [ebp - 4], edx; // josh - move args into local variable
        pop edx;

        mov edx, [ebp - 4];
        test edx, edx;
        jne skip_calc;

        mov eax, [ebp + 8];
        add eax, [ebp + 12];
        add eax, [ebp + 16];
        jmp epilogue;

        skip_calc:
        mov eax, [ebp + 20];

        epilogue:
        mov esp, ebp;
        pop ebp;
        ret;
    }
}

__declspec(naked) int __cdecl subtract(int a)
{
    __asm
    {
        push ebp;
        mov ebp, esp;

        sub [ebp + 8], 50;
        mov eax, [ebp + 8];

        mov esp, ebp;
        pop ebp;
        ret;
    }
}

__declspec(naked) int __cdecl multiply(int a, int mul)
{
    __asm
    {
        push ebp;
        mov ebp, esp;
        sub esp, 4;

        mov eax, [ebp + 12];
        mov dword ptr [ebp - 4], eax;
        mov ebx, [ebp - 4];

        mov eax, [ebp + 8];

        mul ebx;

        mov esp, ebp;
        pop ebp;
        ret;
    }
}

int main()
{
    int cool{}, con_handle{}, msg_len, title_len{}, multiplicator{};
    char buffer[32], title[16];

    std::string window_title{"learning asm"};

    int haha;

    // josh - let's do our calculation
    __asm
    {
        push 0; // josh - skip and add below?
        push 1000;
        push 7;
        push 4;
        push 2;

        call sum;
        add esp, 20;

        push eax;
        call subtract;
        add esp, 4;

        mov dword ptr multiplicator, 2;

        push multiplicator;
        push eax;
        call multiply;
        add esp, 8;

        mov cool, eax;
    }

    msg_len = sprintf(buffer, "%d\n", haha); // NOLINT I know it's deprecated but "sprintf_s" sucks

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
        call WriteConsoleA; // josh - It's an __stdcall so we don't have to clean up the stack!
    }

    // josh - small animation here
    for (int i = 0; i < window_title.length(); i++)
    {
        strncpy_s(title, sizeof(title), window_title.c_str(), i + 1);
        title[i + 1] = '\0';

        __asm
        {
            lea eax, title;
            push eax;
            call SetConsoleTitleA;
        }

        __asm
        {
            push 100;
            call Sleep;
        }
    }

    return 0;
}
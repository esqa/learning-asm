#include "main.h"

#include <format>
#include <iostream>
#include <string>

#include "Windows.h"

static int g_out_handle;
static int g_in_handle;

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

// josh, this is faster because we're not using the stack at all, registers are pretty much the fastest we can go
// there might be a SIMD instruction that I'm not aware of but I'll learn about them in the future, the biggest issue
// using non-standard calling conventions is that we have to setup the call ourselves, ofcourse inlining this will be
// even faster since we don't have to set up the stackframe, saving us a minimum of 5 instructions
// (call, prologue, epilogue) I'm just testing stuff! :D thinking about all of this now, I'll try to write the fastest
// asm i can starting with multiply
__declspec(naked) int subtract_fast(int a, int b, int c)
{
    __asm
    {
        push ebp;
        mov ebp, esp;

        sub eax, edx;
        sub eax, esi;

        mov esp, ebp;
        pop ebp;
        ret;
    }
}

__declspec(naked) int __cdecl subtract(int a, int b)
{
    __asm
    {
        push ebp;
        mov ebp, esp;

        mov edx, [ebp + 12];

        sub [ebp + 8], edx;
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

// josh - no arguments are needed since we know which arguments will be in the corresponding registers
// simply because we have to setup the call ourselves, i'm adding another argument for learning purposes
__declspec(naked) int multiply_fast()
{
    __asm
    {
        push ebp;
        mov ebp, esp;

        mul esi;
        add eax, ecx;

        mov esp, ebp;
        pop ebp;
        ret;
    }
}

void print_string(const char* str)
{
    int out_handle{}, msg_length{};

    char buffer[512];

    msg_length = sprintf(buffer, "%s\n", str); // NOLINT i don't mind it being deprecated!!

    __asm
    {
        push 0;
        push 0;
        push msg_length;

        lea eax, buffer;
        push eax;
        push g_out_handle;
        call WriteConsoleA;
    }
}

int main()
{
    std::string window_title{"learning asm"};

    int title_len{}, retval{};
    char title[16];

    // josh - get our handles here!
    __asm
    {
        push -10;
        call GetStdHandle;
        mov g_in_handle, eax;

        push -11;
        call GetStdHandle;
        mov g_out_handle, eax;
    }

    __asm
    {
        mov eax, 100;
        mov esi, 2; // multiply eax by 2
        mov ecx, 50; // add 50 on top
        call multiply_fast; // we don't have to adjust esp since we didn't have to push anything onto the stack!
        mov retval, eax;
    }

    print_string(std::to_string(retval).c_str());


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

        push 10; // josh - subtract by how much
        push eax;
        call subtract;
        add esp, 8;

        push 2; // josh - multiply by how much
        push eax;
        call multiply;
        add esp, 8;

        mov retval, eax;
    }

    print_string(std::to_string(retval).c_str());

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
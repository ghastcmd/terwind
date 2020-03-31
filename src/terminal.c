#include "terminal.h"

#ifdef _WIN32

#include <windows.h>
// #include <consoleapi.h>

static HANDLE hd_stdout;
static DWORD out_mode_init;

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

void terminal_setup()
{
    DWORD out_mode = 0;
    hd_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hd_stdout == INVALID_HANDLE_VALUE)
    {
        exit(GetLastError());
    }

    if (!GetConsoleMode(hd_stdout, &out_mode))
    {
        exit(GetLastError());
    }

    out_mode_init = out_mode;

    out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hd_stdout, out_mode))
    {
        exit(GetLastError());
    }
}

void terminal_reset()
{
    printf("\x1b[0m\x1b[0d");

    if(!SetConsoleMode(hd_stdout, out_mode_init))
    {
        exit(GetLastError());
    }
}

#else

#include <termios.h>
#include <unistd.h>

void terminal_setup()
{

}

void terminal_reset()
{
    
}

#endif
#include "pch.h"
#include "terminal.h"

#ifdef _WIN32

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void terminal_setup()
{
    DWORD out_mode = 0;
    HANDLE hd_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleMode(hd_stdout, &out_mode);
    
    out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hd_stdout, out_mode);
}

void terminal_reset()
{
    printf("\x1b[0m\x1b[0d"); // resets color and cursor position

    DWORD out_mode = 0;
    HANDLE hd_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleMode(hd_stdout, &out_mode);

    out_mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hd_stdout, out_mode);
}

#else

void terminal_setup()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);

    term.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    setbuf(stdout, NULL); //  disables stdout buffer
}

void terminal_reset()
{
    printf("\x1b[0m\x1b[0d");

    struct termios term;

    tcgetattr(STDIN_FILENO, &term);

    term.c_lflag |= (ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int getch(void)
{
    char ret;
    read(STDIN_FILENO, &ret, 1);
    return ret;
}

int kbhit (void)
{
    int remaining_bytes;
    ioctl(STDIN_FILENO, FIONREAD, &remaining_bytes);
    return remaining_bytes > 0;
}

#endif